#include "xrtc/media/chain/xrtc_preview.h"
#include <rtc_base/logging.h>
#include <rtc_base/task_utils/to_queued_task.h>

#include "xrtc/base/xrtc_global.h"
#include "xrtc/base/xrtc_json.h"

namespace xrtc {

XRTCPreview::XRTCPreview(IVideoSource* video_source, XRTCRender* render) : 
						  current_thread_(rtc::Thread::Current()),
						  video_source_(video_source), 
						  render_(render),
						  xrtc_video_source_(std::make_unique<XRTCVideoSource>()),
						  d3d9_render_sink_(std::make_unique<D3D9RenderSink>()){

}

XRTCPreview::~XRTCPreview() {

}

void XRTCPreview::Start() {
	RTC_LOG(LS_INFO) << "XRTCPreview Start call";
	current_thread_->PostTask(webrtc::ToQueuedTask([=]() {
		RTC_LOG(LS_INFO) << "XRTCPreview Start PostTask";
		XRTCError err = XRTCError::kNoErr;
		do {
			if (has_start_) {
				RTC_LOG(LS_WARNING) << "XRTCPreview already started, ignore";
				break;
			}

			if (!video_source_) {
				RTC_LOG(LS_WARNING) << "XRTCPreview failed:no video source";
				err = XRTCError::kPreviewNoVideoSourceErr;
				break;
			}
			video_source_->AddConsumer(xrtc_video_source_.get());
			AddMediaObject(xrtc_video_source_.get());
			AddMediaObject(d3d9_render_sink_.get());

			if (!ConnectMediaObjects(xrtc_video_source_.get(), d3d9_render_sink_.get())) {
				err = XRTCError::kChainConnectErr;
				RTC_LOG(LS_WARNING) << "XRTCPreview failed: xrtc video source connect d3d9 render sink error";
				break;
			}
			RTC_LOG(LS_WARNING) << "XRTCPreview xrtc video source connect d3d9 render sink success";
			
			JsonObject json_config;
			JsonObject j_d3d9_render_sink;
			j_d3d9_render_sink["hwnd"] = (long long)render_->canvas();
			json_config["d3d9_render_sink"] = j_d3d9_render_sink;
			SetupChain(JsonValue(json_config).ToJson());
			
			if (!StartChain()) {
				err = XRTCError::kChainStartErr;
				RTC_LOG(LS_WARNING) << "XRTCPreview failed: start chain error";
				break;
			}
			has_start_ = true;
		} while (0);

		if (XRTCGlobal::Intance()->engine_observer()) {
			if (err == XRTCError::kNoErr) {
				XRTCGlobal::Intance()->engine_observer()->OnPreviewSuccess(this);
			} else {
				XRTCGlobal::Intance()->engine_observer()->OnPreviewFailed(this, err);
			}
		}
	}));
}

void XRTCPreview::Stop() {
	current_thread_->PostTask(webrtc::ToQueuedTask([=]() {
		RTC_LOG(LS_INFO) << "XRTCPreview Stop PostTask";
		if (!has_start_) {
			return;
		}

		//如果先停止了设备，再停止预览，此处会crash,因为设备停止后，预览的source已经被移除了
		//待优化
		video_source_->RemoveConsumer(xrtc_video_source_.get());

		StopChain();
		has_start_ = false;
	}));
}

void XRTCPreview::Destroy() {
	current_thread_->PostTask(webrtc::ToQueuedTask([=]() {
		RTC_LOG(LS_INFO) << "XRTCPreview Destroy PostTask";
		delete this;
	}));
}

} // namespace xrtc