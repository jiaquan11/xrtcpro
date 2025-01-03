﻿#include "xrtc/xrtc.h"
#include <rtc_base/logging.h>
#include <rtc_base/task_utils/to_queued_task.h>
#include "xrtc/base/xrtc_global.h"
#include "xrtc/device/cam_impl.h"
#include "xrtc/device/xrtc_render.h"
#include "xrtc/media/chain/xrtc_preview.h"
#include "xrtc/media/base/xrtc_pusher.h"

namespace xrtc {

void XRTCEngine::Init(XRTCEngineObserver* observer) {
//测试代码
#if 0
	/*
	 *Invoke:线程同步执行函数，A线程必须等B线程执行完毕才能继续执行，会阻塞A线程
	 *PostTask:线程异步执行函数，A线程不需要等B线程执行完毕就可以继续执行，不会阻塞A线程
	*/
	/*XRTCGlobal::Intance()->api_thread()->Invoke<void>(RTC_FROM_HERE, [=]() {
		int a = 1;
	});*/

	XRTCGlobal::Intance()->api_thread()->PostTask(webrtc::ToQueuedTask([=]() {
		int a = 1;
	}));
	int b = 2;
#endif

	rtc::LogMessage::LogTimestamps(true);//可以打印时间戳
	rtc::LogMessage::LogThreads(true);//可以打印线程信息
	rtc::LogMessage::LogToDebug(rtc::LS_VERBOSE);//可以打印所有级别的日志

	XRTCGlobal::Intance()->RegisterEngineObserver(observer);
	RTC_LOG(LS_INFO) << "XRTCEngine Init";
}

uint32_t XRTCEngine::GetCameraCount() {
	//同步方式获取摄像头数量
	return XRTCGlobal::Intance()->api_thread()->Invoke<uint32_t>(RTC_FROM_HERE, [=]() {
		return XRTCGlobal::Intance()->video_device_info()->NumberOfDevices();
	});
}

int32_t XRTCEngine::GetCameraInfo(int index, std::string& device_name, std::string& device_id) {
	char name[128];
	char id[128];
	int32_t res = XRTCGlobal::Intance()->api_thread()->Invoke<int32_t>(RTC_FROM_HERE, [&]() {
		return XRTCGlobal::Intance()->video_device_info()->GetDeviceName(index, name, sizeof(name), id, sizeof(id));
	});
	device_name = name;
	device_id = id;
	return res;
}

IVideoSource* XRTCEngine::CreateCamSource(const std::string& cam_id) {
	//同步方式创建摄像头资源类
	return XRTCGlobal::Intance()->api_thread()->Invoke<IVideoSource*>(RTC_FROM_HERE, [=]() {
		return new CamImpl(cam_id);
	});
}

XRTCRender* XRTCEngine::CreateRender(void* canvas) {
	//同步方式创建渲染类
	return XRTCGlobal::Intance()->api_thread()->Invoke<XRTCRender*>(RTC_FROM_HERE, [=]() {
		return new XRTCRender(canvas);
	});
}

XRTCPreview* XRTCEngine::CreatePreview(IVideoSource* video_source, XRTCRender* render) {
	//同步方式创建预览类
	return XRTCGlobal::Intance()->api_thread()->Invoke<XRTCPreview*>(RTC_FROM_HERE, [=]() {
		return new XRTCPreview(video_source, render);
	});
}

XRTCPusher* XRTCEngine::CreatePusher(IVideoSource* video_source) {
	//同步方式创建推流类
	return XRTCGlobal::Intance()->api_thread()->Invoke<XRTCPusher*>(RTC_FROM_HERE, [=]() {
		return new XRTCPusher(video_source);
	});
}

} // namespace xrtc