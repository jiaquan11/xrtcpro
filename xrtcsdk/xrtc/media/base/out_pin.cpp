#include "xrtc/media/base/out_pin.h"
#include "xrtc/media/base/media_chain.h"
#include "xrtc/media/base/in_pin.h"

namespace xrtc {

OutPin::OutPin(MediaObject* obj) : BasePin(obj) {

}

OutPin::~OutPin() {

}

bool OutPin::ConnectTo(InPin* in_pin) {
	if (!in_pin || !in_pin->Accept(this)) {
		return false;
	}
	in_pin_ = in_pin;
	return true;
}

void OutPin::PushMediaFrame(std::shared_ptr<MediaFrame> frame) {
	if (in_pin_) {
		in_pin_->PushMediaFrame(frame);
	}
}

} // namespace xrtc