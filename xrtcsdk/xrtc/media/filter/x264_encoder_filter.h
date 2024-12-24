#ifndef XRTCSDK_XRTC_MEDIA_FILTER_X264_ENCODER_FILTER_H_
#define XRTCSDK_XRTC_MEDIA_FILTER_X264_ENCODER_FILTER_H_

#include <queue>
#include <mutex>
#include "xrtc/media/base/media_chain.h"

namespace xrtc {
	
class X264EncoderFilter : public MediaObject {
public:
	X264EncoderFilter();
	~X264EncoderFilter() override;

	//MediaObject implementation
	bool Start() override;
	//void Setup(const std::string& /*json_config*/) {}
	void Stop() override;
	void OnNewMediaFrame(std::shared_ptr<MediaFrame>) override;
	std::vector<InPin*> GetAllInPins() override {
		return std::vector<InPin*>({ in_pin_.get() });
	}
	std::vector<OutPin*> GetAllOutPins() override{
		return std::vector<OutPin*>({ out_pin_.get() });
	}

private:
	std::unique_ptr<InPin> in_pin_;
	std::unique_ptr<OutPin> out_pin_;
	std::queue<std::shared_ptr<MediaFrame>> frame_queue_;
	std::mutex frame_queue_mutex_;
	std::atomic<bool> running_{ false };
	std::thread* encode_thread_{ nullptr };
	std::condition_variable cond_var_;
};

}  // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_FILTER_X264_ENCODER_FILTER_H_
