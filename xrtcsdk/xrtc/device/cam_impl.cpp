#include "xrtc/device/cam_impl.h"

#include <rtc_base/logging.h>
#include <rtc_base/task_utils/to_queued_task.h>
#include <rtc_base/time_utils.h>
#include <modules/video_capture/video_capture_factory.h>

#include "xrtc/base/xrtc_global.h"
#include "xrtc/media/base/media_frame.h"

namespace xrtc {

CamImpl::CamImpl(const std::string& cam_id) :
		         cam_id_(cam_id),
				 current_thread_(rtc::Thread::Current()),
				 device_info_(XRTCGlobal::Intance()->video_device_info()){//保存当前线程的指针

}

CamImpl::~CamImpl() {
	
}

void CamImpl::Start() {
	RTC_LOG(LS_INFO) << "CamImpl Start call";
	//在当前线程(api_thread)中执行PostTask,异步执行
	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		RTC_LOG(LS_INFO) << "CamImpl Start PostTask";
		XRTCError err = XRTCError::kNoErr;
		do {
			if (has_start_) {
				RTC_LOG(LS_WARNING) << "CamImpl has started, ignore";
				break;
			}
			//创建video capture
			video_capture_ = webrtc::VideoCaptureFactory::Create(cam_id_.c_str());
			if (!video_capture_) {
				RTC_LOG(LS_WARNING) << "CamImpl create video capture error";
				err = XRTCError::kVideoCreateCaptureErr;
				break;
			}
			//判断摄像头采集的能力
			if (device_info_->NumberOfCapabilities(cam_id_.c_str()) <= 0) {
				err = XRTCError::kVideoNoCapabilitiesErr;
				RTC_LOG(LS_WARNING) << "CamImpl no capabilities";
				break;
			}
			//获取一个最佳匹配的摄像头能力
			webrtc::VideoCaptureCapability request_cap;
			request_cap.width = 640;
			request_cap.height = 480;
			request_cap.maxFPS = 20;
			webrtc::VideoCaptureCapability best_cap;
			if (device_info_->GetBestMatchedCapability(cam_id_.c_str(), request_cap, best_cap) < 0) {
				err = XRTCError::kVideoNoBestCapabilitiesErr;
				RTC_LOG(LS_WARNING) << "CamImpl no best capabilities";
				break;
			}
			//采集的数据会回调到OnFrame，这里注册回调，当前对象监听
			video_capture_->RegisterCaptureDataCallback(this);
			//启动摄像头采集
			if (video_capture_->StartCapture(best_cap) < 0) {
				err = XRTCError::kVideoStartCaptureErr;
				RTC_LOG(LS_WARNING) << "CamImpl video start capture error";
				break;
			}
			has_start_ = true;
		} while (0);

		//回调启动结果
		if (err != XRTCError::kNoErr) {
			if (XRTCGlobal::Intance()->engine_observer()) {
				XRTCGlobal::Intance()->engine_observer()->OnVideoSourceFailed(this, err);
			}
		} else {
			if (XRTCGlobal::Intance()->engine_observer()) {
				XRTCGlobal::Intance()->engine_observer()->OnVideoSourceSuccess(this);
			}
		}
	}));
}

void CamImpl::Setup(const std::string& json_config) {

}

//停止摄像头采集
void CamImpl::Stop() {
	RTC_LOG(LS_INFO) << "CamImpl Stop call";
	//在当前线程中执行PostTask,异步执行
	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		RTC_LOG(LS_INFO) << "CamImpl Stop PostTask";
		if (!has_start_) {
			return;
		}
		if (video_capture_ && video_capture_->CaptureStarted()) {
			video_capture_->StopCapture();
			video_capture_ = nullptr;
		}
		has_start_ = false;
	}));
}

void CamImpl::Destroy() {
	RTC_LOG(LS_INFO) << "CamImpl Destroy call";
	//在当前线程中执行PostTask,异步执行
	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		RTC_LOG(LS_INFO) << "CamImpl Destroy PostTask";
		delete this;//释放当前对象资源
	}));
}

void CamImpl::AddConsumer(IXRTCConsumer* consumer) {
	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		RTC_LOG(LS_INFO) << "CamImpl Add Consumer" << consumer;
		consumer_list_.push_back(consumer);
	}));
}

void CamImpl::RemoveConsumer(IXRTCConsumer* consumer) {
	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		RTC_LOG(LS_INFO) << "CamImpl Remove Consumer" << consumer;
		auto iter = consumer_list_.begin();
		for (; iter != consumer_list_.end(); ++iter) {
			if (*iter == consumer) {
				consumer_list_.erase(iter);
				break;
			}
		}
	}));
}

void CamImpl::OnFrame(const webrtc::VideoFrame& frame) {
	if (0 == last_frame_ts_) {
		last_frame_ts_ = rtc::Time();
	}
	fps_++;
	int64_t now = rtc::Time();
	if (now - last_frame_ts_ > 1000) {
		RTC_LOG(LS_INFO) << "CamImpl fps: " << fps_;
		last_frame_ts_ = now;
		fps_ = 0;
	}

	int src_width = frame.width();
	int src_height = frame.height();
	int stridey = frame.video_frame_buffer()->GetI420()->StrideY();
	int strideu = frame.video_frame_buffer()->GetI420()->StrideU();
	int stridev = frame.video_frame_buffer()->GetI420()->StrideV();
	//Y + U + V
	int size = stridey * src_height + (strideu + stridev) * ((src_height + 1) / 2);
	std::shared_ptr<MediaFrame> video_frame = std::make_shared<MediaFrame>(size);
	video_frame->fmt.media_type = MainMediaType::kMainTypeVideo;
	video_frame->fmt.sub_fmt.video_fmt.type = SubMediaType::kSubTypeI420;
	video_frame->fmt.sub_fmt.video_fmt.width = src_width;
	video_frame->fmt.sub_fmt.video_fmt.height = src_height;
	video_frame->stride[0] = stridey;
	video_frame->stride[1] = strideu;
	video_frame->stride[2] = stridev;
	video_frame->data_len[0] = stridey * src_height;
	video_frame->data_len[1] = strideu * ((src_height + 1) / 2);
	video_frame->data_len[2] = stridev * ((src_height + 1) / 2);
	video_frame->data[1] = video_frame->data[0] + video_frame->data_len[0];
	video_frame->data[2] = video_frame->data[1] + video_frame->data_len[1];
	memcpy(video_frame->data[0], frame.video_frame_buffer()->GetI420()->DataY(), video_frame->data_len[0]);
	memcpy(video_frame->data[1], frame.video_frame_buffer()->GetI420()->DataU(), video_frame->data_len[1]);
	memcpy(video_frame->data[2], frame.video_frame_buffer()->GetI420()->DataV(), video_frame->data_len[2]);
	
	if (0 == start_time_) {
		start_time_ = frame.render_time_ms();
	}
	video_frame->ts = static_cast<uint32_t>(frame.render_time_ms() - start_time_);

	current_thread_->PostTask(webrtc::ToQueuedTask([=] {
		for (auto consumer : consumer_list_) {
			consumer->OnFrame(video_frame);
		}
	}));
}

} // namespace xrtc