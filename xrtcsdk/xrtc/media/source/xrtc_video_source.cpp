#include "xrtc/media/source/xrtc_video_source.h"
#include "xrtc/media/base/out_pin.h"
#include <rtc_base/logging.h>

namespace xrtc {

XRTCVideoSource::XRTCVideoSource() :
	out_pin_(std::make_unique<OutPin>(this)) {
	MediaFormat fmt;
	fmt.media_type = MainMediaType::kMainTypeVideo;
	fmt.sub_fmt.video_fmt.type = SubMediaType::kSubTypeI420;
	out_pin_->set_format(fmt);
}

XRTCVideoSource::~XRTCVideoSource() {

}

bool XRTCVideoSource::Start() {

	return true;
}

void XRTCVideoSource::Stop() {
	RTC_LOG(LS_INFO) << "XRTCVideoSource Stop";
}

void XRTCVideoSource::OnFrame(std::shared_ptr<MediaFrame> frame) {
	if (out_pin_) {
		out_pin_->PushMediaFrame(frame);
	}
}

} // namespace xrtc