#ifndef XRTCSDK_XRTC_MEDIA_BASE_MEDIA_CHANNEL_H_
#define XRTCSDK_XRTC_MEDIA_BASE_MEDIA_CHANNEL_H_

#include <string>
#include <vector>
#include "xrtc/xrtc.h"

namespace xrtc {

class InPin;
class OutPin;

class MediaObject {
public:
	virtual ~MediaObject(){};

public:
	virtual bool Start() = 0;
	virtual void Setup(const std::string& /*json_config*/) {}
	virtual void Stop() = 0;
	virtual void OnNewMediaFrame(std::shared_ptr<MediaFrame>) {}
	virtual std::vector<InPin*> GetAllInPins() = 0;
	virtual std::vector<OutPin*> GetAllOutPins() = 0;
};

class XRTC_API MediaChain {
public:
	MediaChain() = default;
	virtual ~MediaChain() = default;

public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Destroy() = 0;

protected:
	void AddMediaObject(MediaObject* obj);
	bool ConnectMediaObjects(MediaObject* from, MediaObject* to);
	void SetupChain(const std::string& json_config);
	bool StartChain();
	void StopChain();

private:
	std::vector<MediaObject*> media_objects_;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_BASE_MEDIA_CHANNEL_H_