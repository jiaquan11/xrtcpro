#include "xrtc/xrtc.h"
#include <rtc_base/logging.h>

namespace xrtc {
	int test() {
		rtc::LogMessage::ConfigureLogging("thread");
	}
} // namespace xrtc