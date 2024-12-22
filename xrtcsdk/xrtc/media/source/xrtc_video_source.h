#ifndef XRTCSDK_XRTC_MEDIA_SOURCE_XRTC_VIDEO_SOURCE_H_
#define XRTCSDK_XRTC_MEDIA_SOURCE_XRTC_VIDEO_SOURCE_H_

#include "xrtc/xrtc.h"
#include "xrtc/media/base/media_chain.h"

namespace xrtc {

class OutPin;

class XRTCVideoSource : public IXRTCConsumer,
						public MediaObject{
public:
	XRTCVideoSource();
	~XRTCVideoSource() override;

public:
	//MediaObject
	bool Start() override;
	void Stop() override;
	std::vector<InPin*> GetAllInPins() override {
		return std::vector<InPin*>();
	}
	std::vector<OutPin*> GetAllOutPins() override {
		return std::vector<OutPin*>({out_pin_.get()});
	}

public:
	// IXRTCConsumer
	void OnFrame(std::shared_ptr<MediaFrame> frame) override;

private:
	std::unique_ptr<OutPin> out_pin_;
};

} // namespace xrtc

#endif //XRTCSDK_XRTC_MEDIA_SOURCE_XRTC_VIDEO_SOURCE_H_
