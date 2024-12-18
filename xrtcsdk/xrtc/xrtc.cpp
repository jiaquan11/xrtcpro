#include "xrtc/xrtc.h"
#include <rtc_base/logging.h>
#include <rtc_base/task_utils/to_queued_task.h>
#include "xrtc/base/xrtc_global.h"

namespace xrtc {

void XRTCEngine::Init() {
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
}

} // namespace xrtc