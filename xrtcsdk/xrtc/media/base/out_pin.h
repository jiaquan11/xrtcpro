#ifndef XRTCSDK_XRTC_MEDIA_BASE_OUT_PIN_H_ 
#define XRTCSDK_XRTC_MEDIA_BASE_OUT_PIN_H_

#include <memory>
#include "xrtc/media/base/base_pin.h"

namespace xrtc {

class InPin;

class OutPin : public BasePin {
public:
	OutPin() = delete;
	explicit OutPin(MediaObject* obj);
	~OutPin() override;
	 
public:
	bool ConnectTo(InPin* in_pin);
	// BasePin
	void PushMediaFrame(std::shared_ptr<MediaFrame> frame) override;

private:
	InPin* in_pin_ = nullptr;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_BASE_OUT_PIN_H_
