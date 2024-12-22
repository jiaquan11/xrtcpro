#ifndef XRTCSDK_XRTC_MEDIA_BASE_BASE_PIN_H_ 
#define XRTCSDK_XRTC_MEDIA_BASE_BASE_PIN_H_

#include <memory>
#include "xrtc/media/base/media_frame.h"

namespace xrtc {

class MediaObject;

class BasePin {
public:
	BasePin(MediaObject* obj) : obj_(obj) {}
	virtual ~BasePin() {}

public:
	MediaObject* GetMediaObject() const { return obj_; }
	void set_format(const MediaFormat& fmt) { fmt_ = fmt; }
	MediaFormat format() const { return fmt_; }

	virtual void PushMediaFrame(std::shared_ptr<MediaFrame> frame) = 0;

protected:
	MediaObject* obj_;
	MediaFormat fmt_;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_BASE_BASE_PIN_H_
