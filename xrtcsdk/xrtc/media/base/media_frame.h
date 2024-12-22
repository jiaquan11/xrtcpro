#ifndef XRTCSDK_XRTC_MEDIA_BASE_MEDIA_FRAME_H_
#define XRTCSDK_XRTC_MEDIA_BASE_MEDIA_FRAME_H_

namespace xrtc {

enum class MainMediaType {
	kMainTypeCommon,
	kMainTypeAudio,
	kMainTypeVideo,
	kMainTypeData,
};

enum class SubMediaType {
	kSubTypeCommon,
	kSubTypeI420,
};

struct AudioFormat {
	SubMediaType type;
};

struct VideoFormat {
	SubMediaType type;
	int width;
	int height;
	bool idr;
};

class MediaFormat {
public:
	MainMediaType media_type;
	union {
		AudioFormat audio_fmt;
		VideoFormat video_fmt;
	}sub_fmt;
};

class MediaFrame {
public:
	MediaFrame(int size) : max_size(size) {
		memset(data, 0, sizeof(data));
		memset(data_len, 0, sizeof(data_len));
		memset(stride, 0, sizeof(stride));
		data[0] = new char[size];
		data_len[0] = size;
	}
	~MediaFrame() {
		if (data[0]) {
			delete[] data[0];
			data[0] = nullptr;
		}
	}

public:
	int max_size;
	MediaFormat fmt;
	char* data[4];
	int data_len[4];
	int stride[4];
	uint32_t ts = 0;
};

} // namespace xrtc

#endif // XRTCSDK_XRTC_MEDIA_BASE_MEDIA_FRAME_H_