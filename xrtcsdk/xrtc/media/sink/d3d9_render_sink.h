#ifndef XRTCSDK_XRTC_MEDIA_SINK_D3D9_RENDER_SINK_H_
#define XRTCSDK_XRTC_MEDIA_SINK_D3D9_RENDER_SINK_H_

#include <windows.h>
#include "xrtc/media/base/media_chain.h"

struct IDirect3D9;
struct IDirect3DDevice9;
struct IDirect3DSurface9;

namespace xrtc {

class InPin;

class D3D9RenderSink : public MediaObject{
public:
	D3D9RenderSink();
	~D3D9RenderSink() override;

public:
	//MediaObject
	bool Start() override;
	void Setup(const std::string& json_config) override;
	void Stop() override;
	std::vector<InPin*> GetAllInPins() override {
		return std::vector<InPin*>({in_pin_.get()});
	}

	std::vector<OutPin*> GetAllOutPins() override {
		return std::vector<OutPin*>();
	}

	void OnNewMediaFrame(std::shared_ptr<MediaFrame> frame) override;

private:
	bool TryInit(std::shared_ptr<MediaFrame> frame);
	void DoRender(std::shared_ptr<MediaFrame> frame);

private:
	std::unique_ptr<InPin> in_pin_;
	HWND hwnd_ = nullptr;

	IDirect3D9* d3d9_ = nullptr;
	IDirect3DDevice9* d3d9_device_ = nullptr;
	IDirect3DSurface9* d3d9_surface_ = nullptr;

	int width_  = 640;
	int height_ = 480;
	RECT rt_viewport_;

	char* rgb_buffer_ = nullptr;
	int rgb_buffer_size_ = 0;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_SINK_D3D9_RENDER_SINK_H_
