#include "xrtc/media/base/in_pin.h"
#include "xrtc/media/base/media_chain.h"
#include "xrtc/media/base/out_pin.h"

namespace xrtc {

InPin::InPin(MediaObject* obj) : BasePin(obj){

}

InPin::~InPin() {

}

bool InPin::Accept(OutPin* out_pin) {
	if (!out_pin) {
		return false;
	}
	//比较主类型
	MediaFormat out_fmt = out_pin->format();
	if (out_fmt.media_type == MainMediaType::kMainTypeCommon || fmt_.media_type == MainMediaType::kMainTypeCommon) {
		out_pin_ = out_pin;
		return true;
	} else if (out_fmt.media_type != fmt_.media_type) {
		return false;
	}
	//比较子类型
	switch (out_fmt.media_type) {
	case MainMediaType::kMainTypeAudio:
	{
		if (out_fmt.sub_fmt.audio_fmt.type == SubMediaType::kSubTypeCommon || fmt_.sub_fmt.audio_fmt.type == SubMediaType::kSubTypeCommon) {
			out_pin_ = out_pin;
			return true;
		}
		else if (out_fmt.sub_fmt.audio_fmt.type != fmt_.sub_fmt.audio_fmt.type) {
			return false;
		}
		break;
	}
	case MainMediaType::kMainTypeVideo:
	{
		if (out_fmt.sub_fmt.video_fmt.type == SubMediaType::kSubTypeCommon || fmt_.sub_fmt.video_fmt.type == SubMediaType::kSubTypeCommon) {
			out_pin_ = out_pin;
			return true;
		}
		else if (out_fmt.sub_fmt.video_fmt.type != fmt_.sub_fmt.video_fmt.type) {
			return false;
		}
		break;
	}
	default:
		return false;
	}
	out_pin_ = out_pin;
	return true;
}

void InPin::PushMediaFrame(std::shared_ptr<MediaFrame> frame) {
	if (obj_) {
		obj_->OnNewMediaFrame(frame);
	}
}

} // namespace xrtc