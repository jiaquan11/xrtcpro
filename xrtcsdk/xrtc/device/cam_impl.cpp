#include "xrtc/device/cam_impl.h"

#include <rtc_base/logging.h>
#include <rtc_base/task_utils/to_queued_task.h>
#include <rtc_base/time_utils.h>
#include <modules/video_capture/video_capture_factory.h>

#include "xrtc/base/xrtc_global.h"

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

}

void CamImpl::RemoveConsumer(IXRTCConsumer* consumer) {

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
}

} // namespace xrtc