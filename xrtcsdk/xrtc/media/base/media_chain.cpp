#include "xrtc/media/base/media_chain.h"
#include "xrtc/media/base/in_pin.h"
#include "xrtc/media/base/out_pin.h"

namespace xrtc {

void MediaChain::AddMediaObject(MediaObject* obj) {
	media_objects_.push_back(obj);
}

bool MediaChain::ConnectMediaObjects(MediaObject* from, MediaObject* to) {
	if (!from || !to) {
		return false;
	}
	std::vector<OutPin*> out_pins = from->GetAllOutPins();
	std::vector<InPin*> in_pins = to->GetAllInPins();
	for (auto out_pin : out_pins) {
		bool has_connected = false;
		for (auto in_pin : in_pins) {
			if (out_pin->ConnectTo(in_pin)) {
				has_connected = true;
				break;
			}
		}
		if (!has_connected) {
			return false;
		}
	}
	return true;
}

void MediaChain::SetupChain(const std::string& json_config) {
	for (auto obj : media_objects_) {
		obj->Setup(json_config);
	}
}

bool MediaChain::StartChain() {
	for (auto obj : media_objects_) {
		if (!obj->Start()) {
			return false;
		}
	}
	return true;
}

void MediaChain::StopChain() {
	for (auto obj : media_objects_) {
		obj->Stop();
	}
}

}  // namespace xrtc