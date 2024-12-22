#ifndef XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PREVIEW_H_ 
#define XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PREVIEW_H_

#include <rtc_base/thread.h>
#include "xrtc/media/base/media_chain.h"
#include "xrtc/xrtc.h"
#include "xrtc/device/xrtc_render.h"
#include "xrtc/media/source/xrtc_video_source.h"
#include "xrtc/media/sink/d3d9_render_sink.h"

namespace xrtc {

class XRTC_API XRTCPreview : public MediaChain {
public:
	~XRTCPreview();

public:
	void Start() override;
	void Stop() override;
	void Destroy() override;

private:
	XRTCPreview(IVideoSource* video_source, XRTCRender* render);
	friend class XRTCEngine;

private:
	rtc::Thread* current_thread_;
	IVideoSource* video_source_;
	XRTCRender* render_;
	std::unique_ptr<XRTCVideoSource> xrtc_video_source_;
	std::unique_ptr<D3D9RenderSink> d3d9_render_sink_;
	bool has_start_ = false;
};

} // namespace xrtc

#endif //XRTCSDK_XRTC_MEDIA_CHAIN_XRTC_PREVIEW_H_
