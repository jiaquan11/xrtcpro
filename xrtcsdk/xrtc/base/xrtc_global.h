#ifndef XRTCSDK_XRTC_BASE_XRTC_GLOBAL_H_
#define XRTCSDK_XRTC_BASE_XRTC_GLOBAL_H_

#include <rtc_base/thread.h>
#include <modules/video_capture/video_capture.h>

namespace xrtc {
class XRTCEngineObserver;

//单例模式
class XRTCGlobal {
public:
	static XRTCGlobal* Intance();

	XRTCEngineObserver* engine_observer() { return engine_observer_; }
	void RegisterEngineObserver(XRTCEngineObserver* observer) { engine_observer_ = observer; }

	rtc::Thread* api_thread() { return api_thread_.get(); }
	rtc::Thread* worker_thread() { return worker_thread_.get(); }
	rtc::Thread* network_thread() { return network_thread_.get(); }
	webrtc::VideoCaptureModule::DeviceInfo* video_device_info() { return video_device_info_.get(); }

private:
	//构造函数设置为私有:防止外部直接创建对象，确保对象的生命周期由类自身管理
	XRTCGlobal();
	//析构函数设置为私有:防止外部直接delete对象，确保对象的生命周期由类自身管理
	~XRTCGlobal();

private:
	std::unique_ptr<rtc::Thread> api_thread_;
	std::unique_ptr<rtc::Thread> worker_thread_;
	std::unique_ptr<rtc::Thread> network_thread_;
	std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> video_device_info_;
	XRTCEngineObserver* engine_observer_ = nullptr;
};

} // namespace xrtc

#endif
