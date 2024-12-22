#ifndef XRTCSDK_XRTC_MEDIA_BASE_IN_PIN_H_ 
#define XRTCSDK_XRTC_MEDIA_BASE_IN_PIN_H_

#include "xrtc/media/base/base_pin.h"

namespace xrtc {

class OutPin;

class InPin : public BasePin {
public:
	InPin() = delete;
	explicit InPin(MediaObject* obj);
	~InPin() override;

public:
	bool Accept(OutPin* out_pin);
	// BasePin
	void PushMediaFrame(std::shared_ptr<MediaFrame> frame) override;

private:
	OutPin* out_pin_ = nullptr;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_BASE_IN_PIN_H_
