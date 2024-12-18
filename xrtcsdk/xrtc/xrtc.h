#ifndef XRTCSDK_XRTC_XRTC_H_
#define XRTCSDK_XRTC_XRTC_H_

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

class XRTC_API XRTCEngine {
public:
	static void Init();
};

} // namespace xrtc

#endif //XRTCSDK_XRTC_XRTC_H_