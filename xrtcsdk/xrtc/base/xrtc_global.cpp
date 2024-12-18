#include "xrtc/base/xrtc_global.h"

namespace xrtc {

XRTCGlobal* XRTCGlobal::Intance() {
	static XRTCGlobal* const instance = new XRTCGlobal();
	return instance;
	//ʹ�� std::unique_ptr ����������std::unique_ptr ���ڳ����˳�ʱ�Զ��������������ͷ��ڴ�
	/*static std::unique_ptr<XRTCGlobal> instance(new XRTCGlobal());
	return instance.get();*/

	/*
	��������:
	static XRTCGlobal* const instance = new XRTCGlobal();
	// ע�����ٺ�����ȷ�������˳�ʱ�ͷ��ڴ�
	static bool registered = []() {
		std::atexit([]() {
			delete instance; // �������������ͷ��ڴ�
		});
		return true;
	}();
	return instance;
	*/
}

XRTCGlobal::XRTCGlobal() :
	api_thread_(rtc::Thread::Create()),//����һ����ͨ�߳�
	worker_thread_(rtc::Thread::Create()),//����һ����ͨ�߳�
	network_thread_(rtc::Thread::CreateWithSocketServer()){//����һ����socket���߳�
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