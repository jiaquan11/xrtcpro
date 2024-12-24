#ifndef XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PUSH_STREAM_H_
#define XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PUSH_STREAM_H_

#include <memory>
#include "xrtc/media/source/xrtc_video_source.h"
#include "xrtc/media/base/media_chain.h"
#include "xrtc/media/filter/x264_encoder_filter.h"

namespace xrtc {

class XRTCPusher;

class XRTCPushStream : public MediaChain {
public:
	XRTCPushStream(XRTCPusher* pusher, IVideoSource* video_source);
	~XRTCPushStream() override;

public:
	//MediaChain
	void Start() override;
	void Stop() override;
	void Destroy() override;

private:
	XRTCPusher* pusher_;
	IVideoSource* video_source_;
	std::unique_ptr<XRTCVideoSource> xrtc_video_source_;
	std::unique_ptr<X264EncoderFilter> x264_encoder_filter_;
};

} // namespace xrtc

#endif //XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PUSH_STREAM_H_
