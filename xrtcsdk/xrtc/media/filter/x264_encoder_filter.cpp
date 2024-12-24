#include "xrtc/media/filter/x264_encoder_filter.h"

#include <rtc_base/logging.h>
#include <rtc_base/thread.h>
#include "xrtc/media/base/in_pin.h"
#include "xrtc/media/base/out_pin.h"

namespace xrtc {

X264EncoderFilter::X264EncoderFilter() :
					in_pin_(std::make_unique<InPin>(this)),
					out_pin_(std::make_unique<OutPin>(this)) {
	MediaFormat fmt_in;
	fmt_in.media_type = MainMediaType::kMainTypeVideo;
	fmt_in.sub_fmt.video_fmt.type = SubMediaType::kSubTypeI420;
	in_pin_->set_format(fmt_in);

	MediaFormat fmt_out;
	fmt_out.media_type = MainMediaType::kMainTypeVideo;
	fmt_out.sub_fmt.video_fmt.type = SubMediaType::kSubTypeH264;
}

X264EncoderFilter::~X264EncoderFilter() {

}

bool X264EncoderFilter::Start() {
	RTC_LOG(LS_INFO) << "X264EncoderFilter Start";
	if (running_) {
		RTC_LOG(LS_WARNING) << "X264EncoderFilter already started";
		return true;
	}

	running_ = true;
	encode_thread_ = new std::thread([this]() {
		rtc::SetCurrentThreadName("x264_encode_thread");
		RTC_LOG(LS_INFO) << "X264EncoderFilter encode thread running";
		int frame_queue_size = 0;
		while (running_) {
			std::shared_ptr<MediaFrame> frame;
			{
				std::unique_lock<std::mutex> auto_lock(frame_queue_mutex_);
				frame_queue_size = frame_queue_.size();
				if (frame_queue_size > 0) {
					frame = frame_queue_.front();
					frame_queue_.pop();
				}
				if (!frame) {
					cond_var_.wait(auto_lock);
					continue;
				}
			}
		}
	});
	return false;
}

void X264EncoderFilter::Stop() {

}

void X264EncoderFilter::OnNewMediaFrame(std::shared_ptr<MediaFrame> frame) {
	std::unique_lock<std::mutex> auto_lock(frame_queue_mutex_);
	frame_queue_.push(frame);
	cond_var_.notify_one();
}

} // namespace xrtc