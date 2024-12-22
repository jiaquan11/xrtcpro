#include "xrtc/media/sink/d3d9_render_sink.h"

#include <rtc_base/task_utils/to_queued_task.h>
#include <rtc_base/logging.h>
#include <d3d9.h>
#include <libyuv.h>
#include "xrtc/media/base/in_pin.h"
#include "xrtc/base/xrtc_json.h"
#include "xrtc/base/xrtc_global.h"

namespace xrtc {

D3D9RenderSink::D3D9RenderSink() :
	in_pin_(std::make_unique<InPin>(this)) {
	MediaFormat fmt;
	fmt.media_type = MainMediaType::kMainTypeVideo;
	fmt.sub_fmt.video_fmt.type = SubMediaType::kSubTypeI420;
	in_pin_->set_format(fmt);
}
	
D3D9RenderSink::~D3D9RenderSink() {

}
	
bool D3D9RenderSink::Start() {
	return true;
}

void D3D9RenderSink::Setup(const std::string& json_config) {
	JsonValue value;
	value.FromJson(json_config);
	JsonObject jobject = value.ToObject();
	JsonObject jd3d9 = jobject["d3d9_render_sink"].ToObject();
	hwnd_ = (HWND)jd3d9["hwnd"].ToInt();
}

void D3D9RenderSink::Stop() {
	RTC_LOG(LS_INFO) << "D3D9RenderSink Stop";
	if (d3d9_surface_) {
		d3d9_surface_->Release();
		d3d9_surface_ = nullptr;
	}
	if (d3d9_device_) {
		d3d9_device_->Release();
		d3d9_device_ = nullptr;
	}
	if (d3d9_) {
		d3d9_->Release();
		d3d9_ = nullptr;
	}
	if (rgb_buffer_) {
		delete[] rgb_buffer_;
		rgb_buffer_ = nullptr;
		rgb_buffer_size_ = 0;
	}
}

void D3D9RenderSink::OnNewMediaFrame(std::shared_ptr<MediaFrame> frame) {
	//work_thread执行渲染工作
	XRTCGlobal::Intance()->worker_thread()->PostTask(webrtc::ToQueuedTask([=]() {
		if (!TryInit(frame)) {
			RTC_LOG(LS_WARNING) << "Try Init failed";
			return;
		}

		DoRender(frame);
	}));
}

bool D3D9RenderSink::TryInit(std::shared_ptr<MediaFrame> frame) {
	do {
		if (!d3d9_ || !d3d9_device_ || !d3d9_surface_) {
			break;
		}
		if (width_ != frame->fmt.sub_fmt.video_fmt.width || height_ != frame->fmt.sub_fmt.video_fmt.height) {
			break;
		}
		return true;
	} while (false);
	
	if (!IsWindow(hwnd_)) {
		RTC_LOG(LS_ERROR) << "Invalid hwnd";
		return false;
	}

	if (!GetClientRect(hwnd_, &rt_viewport_)) {
		RTC_LOG(LS_WARNING) << "GetClientRect failed, hwnd:" << hwnd_;
		return false;
	}
	
	//1、创建d3d9对象
	if (!d3d9_) {
		d3d9_ = Direct3DCreate9(D3D_SDK_VERSION);
		if (!d3d9_) {
			RTC_LOG(LS_ERROR) << "Direct3DCreate9 failed";
			return false;
		}
	}

	//2、创建一个d3d9设备
	if (!d3d9_device_) {
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = true; //窗口模式
		//一旦后台缓冲区的内容被显示到前台缓冲区，后台缓冲区的内容就会被丢弃
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;//后台缓冲区的格式，默认为显示器的格式

		HRESULT res = d3d9_->CreateDevice(
			D3DADAPTER_DEFAULT,//指定要表示的物理设备，默认为主显示器
			D3DDEVTYPE_HAL,//指定要创建的设备类型，HAL表示硬件加速设备
			hwnd_,//窗口句柄
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,//顶点处理方式，混合顶点处理方式，多线程
			&d3dpp,//D3DPRESENT_PARAMETERS结构体
			&d3d9_device_//返回创建的设备
		);
		if (FAILED(res)) {
			RTC_LOG(LS_ERROR) << "d3d9 create device failed" << res;
			return false;
		}
	}

	//3、创建离屏表面
	if (d3d9_surface_) {
		d3d9_surface_->Release();
		d3d9_surface_ = nullptr;
	}
	HRESULT res = d3d9_device_->CreateOffscreenPlainSurface(
		frame->fmt.sub_fmt.video_fmt.width,
		frame->fmt.sub_fmt.video_fmt.height,
		D3DFMT_X8R8G8B8,//surface表面数据格式
		D3DPOOL_DEFAULT,
		&d3d9_surface_,//返回创建的表面surface
		nullptr
	);
	if (FAILED(res)) {
		RTC_LOG(LS_ERROR) << "d3d9 create offscreen surface failed" << res;
		return false;
	}
	width_ = frame->fmt.sub_fmt.video_fmt.width;
	height_ = frame->fmt.sub_fmt.video_fmt.height;
	return true;
}

void D3D9RenderSink::DoRender(std::shared_ptr<MediaFrame> frame) {
	//1、创建RGB buffer,将I420数据转换为RGB32
	if (SubMediaType::kSubTypeI420 == frame->fmt.sub_fmt.video_fmt.type) {
		int size = frame->fmt.sub_fmt.video_fmt.width * frame->fmt.sub_fmt.video_fmt.height * 4;
		if (rgb_buffer_size_ != size) {
			if (rgb_buffer_) {
				delete[] rgb_buffer_;
			}
		}
		rgb_buffer_ = new char[size];
		rgb_buffer_size_ = size;
	}
	//YUV格式转换成RGB
	libyuv::I420ToARGB(
		(const uint8_t*)frame->data[0], frame->stride[0],
		(const uint8_t*)frame->data[1], frame->stride[1],
		(const uint8_t*)frame->data[2], frame->stride[2],
		(uint8_t*)rgb_buffer_,
		width_ * 4, width_, height_
	);
	//2、将RGB buffer数据拷贝到d3d9离屏表面
	//2.1 锁定表面区域
	HRESULT res;
	D3DLOCKED_RECT d3d9_rect;
	res = d3d9_surface_->LockRect(&d3d9_rect, //返回锁定的矩形区域
		nullptr, //指定要锁定的矩形区域，nullptr表示锁定整个表面
		D3DLOCK_DONOTWAIT//当显卡锁定区域失败的时候，不阻塞应用程序
	);
	if (FAILED(res)) {
		RTC_LOG(LS_ERROR) << "d3d9 lock rect failed" << res;
		return;
	}
	//2.2 将RGB buffer数据拷贝到d3d9离屏表面
	//锁定区域的首地址
	byte* pdest = (byte*)d3d9_rect.pBits;
	//锁定区域每一行的数据大小
	int stride = d3d9_rect.Pitch;
	if (SubMediaType::kSubTypeI420 == frame->fmt.sub_fmt.video_fmt.type) {
		int video_width = frame->fmt.sub_fmt.video_fmt.width;
		int video_height = frame->fmt.sub_fmt.video_fmt.height;
		int video_stride = video_width * 4;
		if (video_stride == stride) {
			memcpy(pdest, rgb_buffer_, rgb_buffer_size_);
		} else if (video_stride < stride) {
			char* src = rgb_buffer_;
			for (int i = 0; i < video_height; i++) {
				memcpy(pdest, src, video_stride);
				pdest += stride;
				src += video_stride;
			}
		}
	}
	//2.3解除锁定
	d3d9_surface_->UnlockRect();
	//3、清除后台缓冲
	d3d9_device_->Clear(0,//第二个矩形数组参数的大小
		NULL,//需要清除的矩形数组，如果是NULL，表示清除整个后台缓冲区
		D3DCLEAR_TARGET,//需要清除的缓冲区，D3DCLEAR_TARGET表示清除后台缓冲区
		D3DCOLOR_XRGB(30, 30, 30),//使用的颜色值
		1.0f, 0);//深度值，模板值
	//4、将离屏表面数据拷贝到后台缓冲区
	d3d9_device_->BeginScene();
	//获取后台缓存区表面
	IDirect3DSurface9* pback_buffer = nullptr;
	d3d9_device_->GetBackBuffer(0,//正在使用的交换链的索引
		0,//后台缓冲区的索引
		D3DBACKBUFFER_TYPE_MONO,//后台缓冲区的类型
		&pback_buffer);//返回后台缓冲区

	//窗口显示区域的宽高
	float w1 = rt_viewport_.right - rt_viewport_.left;
	float h1 = rt_viewport_.bottom - rt_viewport_.top;
	//图像的宽高
	float w2 = (float)width_;
	float h2 = (float)height_;
	//计算目标区域的大小
	int dst_w = 0;
	int dst_h = 0;
	int x, y = 0;
	if (w1 > (w2 * h1) / h2) {//原始的显示区域，宽度更宽一些
		dst_w = (w2 * h1) / h2;
		dst_h = h1;
		x = (w1 - dst_w) / 2;
		y = 0;
	} else {
		dst_w = w1;
		dst_h = (h2 * w1) / w2;
		x = 0;
		y = (h1 - dst_h) / 2;
	}
	RECT dest_rect{ x, y, x + dst_w, y + dst_h };

	//将离屏表面数据拷贝到后台缓冲区
	d3d9_device_->StretchRect(d3d9_surface_,//源表面
		NULL,//源表面的矩形区域，如果是NULL，表示整个表面
		pback_buffer,//目标表面
		&dest_rect,//目标表面的矩形区域，如果是NULL，表示整个表面
		D3DTEXF_LINEAR);//缩放算法，线性插值
	//结束渲染
	d3d9_device_->EndScene();
	
	//5、交换前后台缓冲区,显示图像，表面翻转
	d3d9_device_->Present(NULL, NULL, NULL, NULL);
	//6、释放后台缓冲
	pback_buffer->Release();
}

} // namespace xrtc