#ifndef XRTCSDK_XRTC_XRTC_H_
#define XRTCSDK_XRTC_XRTC_H_

#include <string>
#include <memory>

#ifdef XRTC_STATIC
#define XRTC_API
#else
#ifdef XRTC_API_EXPORT
#if defined(_MSC_VER)
#define XRTC_API __declspec(dllexport)
#else
#define XRTC_API
#endif
#else
#if defined(_MSC_VER)
#define XRTC_API __declspec(dllimport)
#else
#define XRTC_API
#endif
#endif
#endif

namespace xrtc {

class MediaFrame;
class XRTCRender;
class XRTCPreview;
class XRTCPusher;

enum class XRTCError {
	kNoErr = 0,
	kVideoCreateCaptureErr = -1000,
	kVideoNoCapabilitiesErr,
	kVideoNoBestCapabilitiesErr,
	kVideoStartCaptureErr,
	kPreviewNoVideoSourceErr,
	kChainConnectErr,
	kChainStartErr,
	kPushNoVideoSourceErr,
};

class IXRTCConsumer {
public:
	virtual ~IXRTCConsumer() {}

public:
	virtual void OnFrame(std::shared_ptr<MediaFrame> frame) = 0;
};

class IMediaSource {
public:
	//基类的析构函数必须是虚函数，否则delete指向派生类对象的基类指针时，
	//只会调用基类的析构函数，而不会调用派生类的析构函数，导致派生类对象的资源无法释放。
	virtual ~IMediaSource() {}

public:
	virtual void Start() = 0;
	virtual void Setup(const std::string& json_config) = 0;
	virtual void Stop() = 0;
	virtual void Destroy() = 0;
	virtual void AddConsumer(IXRTCConsumer* consumer)= 0;
	virtual void RemoveConsumer(IXRTCConsumer* consumer) = 0;
};

class IVideoSource : public IMediaSource {

};

class XRTC_API XRTCEngineObserver {
public:
	virtual void OnVideoSourceSuccess(IVideoSource*) {};
	virtual void OnVideoSourceFailed(IVideoSource*, XRTCError) {};
	virtual void OnPreviewSuccess(XRTCPreview*) {};
	virtual void OnPreviewFailed(XRTCPreview*, XRTCError) {};
};

class XRTC_API XRTCEngine {
public:
	static void Init(XRTCEngineObserver* observer);
	static uint32_t GetCameraCount();
	static int32_t GetCameraInfo(int index, std::string& device_name, std::string& device_id);
	static IVideoSource* CreateCamSource(const std::string& cam_id);
	static XRTCRender* CreateRender(void* canvas);
	static XRTCPreview* CreatePreview(IVideoSource* video_source, XRTCRender* render);
	static XRTCPusher* CreatePusher(IVideoSource* video_source);
};

} // namespace xrtc

#endif //XRTCSDK_XRTC_XRTC_H_