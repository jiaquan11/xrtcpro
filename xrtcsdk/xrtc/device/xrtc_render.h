#ifndef XRTCSDK_XRTC_DEVICE_XRTC_RENDER_H_ 
#define XRTCSDK_XRTC_DEVICE_XRTC_RENDER_H_

namespace xrtc {

class XRTCRender {
public:
	~XRTCRender() {}

public:
	void* canvas() { return canvas_; }

private:
	XRTCRender(void* canvas) : canvas_(canvas) {}
	friend class XRTCEngine;

private:
	void* canvas_;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_DEVICE_XRTC_RENDER_H_
