#include "xrtc/base/xrtc_global.h"

namespace xrtc {

XRTCGlobal* XRTCGlobal::Intance() {
	static XRTCGlobal* const instance = new XRTCGlobal();
	return instance;
	//使用 std::unique_ptr 管理单例对象，std::unique_ptr 会在程序退出时自动调用析构函数释放内存
	/*static std::unique_ptr<XRTCGlobal> instance(new XRTCGlobal());
	return instance.get();*/

	/*
	或者如下:
	static XRTCGlobal* const instance = new XRTCGlobal();
	// 注册销毁函数，确保程序退出时释放内存
	static bool registered = []() {
		std::atexit([]() {
			delete instance; // 调用析构函数释放内存
		});
		return true;
	}();
	return instance;
	*/
}

XRTCGlobal::XRTCGlobal() :
	api_thread_(rtc::Thread::Create()),//创建一个普通线程
	worker_thread_(rtc::Thread::Create()),//创建一个普通线程
	network_thread_(rtc::Thread::CreateWithSocketServer()){//创建一个带socket的线程
	api_thread_->SetName("api_thread", nullptr);
	api_thread_->Start();
	worker_thread_->SetName("worker_thread", nullptr);
	worker_thread_->Start();
	network_thread_->SetName("network_thread", nullptr);
	network_thread_->Start();
}

XRTCGlobal::~XRTCGlobal() {

}

} // namespace xrtc