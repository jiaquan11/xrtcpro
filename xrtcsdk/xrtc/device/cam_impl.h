#ifndef XRTCSDK_XRTC_DEVICE_CAM_IMPL_H_
#define XRTCSDK_XRTC_DEVICE_CAM_IMPL_H_

#include <atomic>
#include <rtc_base/thread.h>
#include <modules/video_capture/video_capture.h>
#include "xrtc/xrtc.h"

namespace xrtc {

class CamImpl : public IVideoSource,
			    public rtc::VideoSinkInterface<webrtc::VideoFrame>{
public:
	void Start() override;
	void Setup(const std::string& json_config) override;
	void Stop() override;
	void Destroy() override;
	void AddConsumer(IXRTCConsumer* consumer) override;
	void RemoveConsumer(IXRTCConsumer* consumer) override;

	//rtc::VideoSinkInterface
	void OnFrame(const webrtc::VideoFrame& frame) override;

private:
	CamImpl(const std::string& cam_id);
	~CamImpl();
	
	//设置XRTCEngine为友元类，以便XRTCEngine可以访问CamImpl的私有构造函数
	friend class XRTCEngine;
private:
	std::string cam_id_;
	rtc::Thread* current_thread_;
	bool has_start_ = false;
	rtc::scoped_refptr<webrtc::VideoCaptureModule> video_capture_;
	webrtc::VideoCaptureModule::DeviceInfo* device_info_;
	std::atomic<int> fps_{ 0 };//采集数据回调是多线程的，所以需要使用原子操作
	std::atomic<int64_t> last_frame_ts_{ 0 };
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_DEVICE_CAM_IMPL_H_
