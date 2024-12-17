cmake_minimum_required (VERSION 3.5)

project(libwebrtc)

# 静态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# 动态库输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# 可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_SYSTEM_NAME})

include_directories(
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/third_party/abseil-cpp"
    "${PROJECT_SOURCE_DIR}/third_party/openssl/include"
    "${PROJECT_SOURCE_DIR}/third_party/libyuv/include"
    "${PROJECT_SOURCE_DIR}"
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Current platform: Windows")
    add_definitions(-DUNICODE -D_UNICODE -DWIN32 -D_WIN32 -DWIN32_LEAN_AND_MEAN 
        -DNOMINMAX 
        -D_WINDOWS
        -D_USE_MATH_DEFINES
        -DWEBRTC_WIN
        -DWEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE
        -DWEBRTC_EXTERNAL_JSON
        -DWEBRTC_APM_DEBUG_DUMP)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Current platform: Linux")
    add_definitions("-g -pipe -W -Wall -fPIC -std=gnu++14 -DWEBRTC_POSIX
    -DWEBRTC_LINUX -DNDEBUG -Wno-unused-parameter")
endif()

file(GLOB all_src
    "./src/rtc_base/*.cc"
    "./src/rtc_base/strings/*.cc"
    "./src/rtc_base/synchronization/*.cc"
    "./src/rtc_base/deprecated/*.cc"
    "./src/rtc_base/internal/*.cc"
    "./src/rtc_base/network/*.cc"
    "./src/rtc_base/task_utils/*.cc"
    "./src/rtc_base/memory/*.cc"
    "./src/rtc_base/containers/*.cc"
    "./src/rtc_base/experiments/*.cc"
    "./src/rtc_base/system/*.cc"
    "./src/rtc_base/third_party/sigslot/*.cc"
    "./src/rtc_base/third_party/base64/*.cc"
    "./src/api/*.cc"
    "./src/api/task_queue/*.cc"
    "./src/api/units/*.cc"
    "./src/api/crypto/*.cc"
    "./src/api/audio/*.cc"
    "./src/api/video/*.cc"
    "./src/api/transport/*.cc"
    "./src/api/transport/rtp/*.cc"
    "./src/api/audio_codecs/*.cc"
    "./src/system_wrappers/source/*.cc"
    "./src/modules/video_capture/*.cc"
    "./src/modules/audio_device/*.cc"
    "./src/modules/audio_device/dummy/*.cc"
    "./src/modules/audio_processing/*.cc"
    "./src/modules/audio_processing/include/*.cc"
    "./src/modules/audio_processing/aec3/*.cc"
    "./src/modules/audio_processing/aecm/*.cc"
    "./src/modules/audio_processing/aec_dump/*.cc"
    "./src/modules/audio_processing/agc/*.cc"
    "./src/modules/audio_processing/agc/legacy/*.cc"
    "./src/modules/audio_processing/agc2/*.cc"
    "./src/modules/audio_processing/agc2/rnn_vad/*.cc"
    "./src/modules/audio_processing/vad/*.cc"
    "./src/modules/audio_processing/ns/*.cc"
    "./src/modules/audio_processing/transient/*.cc"
    "./src/modules/audio_processing/capture_levels_adjuster/*.cc"
    "./src/modules/audio_processing/utility/*.cc"
    "./src/modules/audio_processing/logging/*.cc"
    "./src/modules/audio_processing/echo_detector/*.cc"
    "./src/modules/audio_coding/codecs/isac/main/source/*.c"
    "./src/modules/third_party/fft/*.c"
    "./src/modules/rtp_rtcp/include/*.cc"
    "./src/modules/rtp_rtcp/source/*.cc"
    "./src/modules/rtp_rtcp/source/rtcp_packet/*.cc"
    "./src/common_video/*.cc"
    "./src/common_video/libyuv/*.cc"
    "./src/common_video/h264/*.cc"
    "./src/common_audio/*.cc"
    "./src/common_audio/*.c"
    "./src/common_audio/resampler/*.cc"
    "./src/common_audio/signal_processing/*.c"
    "./src/common_audio/signal_processing/*.cc"
    "./src/common_audio/vad/*.c"
    "./src/common_audio/third_party/ooura/fft_size_128/*.cc"
    "./src/common_audio/third_party/ooura/fft_size_256/*.cc"
    "./src/common_audio/third_party/spl_sqrt_floor/*.c"
    "./src/media/base/*.cc"
    "./third_party/pffft/src/*.c"
    "./third_party/rnnoise/src/*.cc"
)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    file(GLOB rtc_base_win
        "./src/rtc_base/win/*.cc"
    )

    file(GLOB api_win
        "./src/api/task_queue/default_task_queue_factory_win.cc"
    )

    file(GLOB video_capture_win
        "./src/modules/video_capture/windows/*.cc"
    )

    file(GLOB audio_device_win
        "./src/modules/audio_device/win/*.cc"
    )

endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_library(libwebrtc STATIC
        ${all_src}
        ${rtc_base_win}
        ${api_win}
        ${audio_device_win}
        ${video_capture_win}
)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    file(GLOB exclude_src
        "./src/rtc_base/win32.cc"
        "./src/rtc_base/task_queue_win.cc"
        "./src/api/task_queue/default_task_queue_factory_win.cc"
    )
    list(REMOVE_ITEM all_src ${exclude_src})
    add_library(libwebrtc 
        ${all_src}
    )
endif()

install(TARGETS libwebrtc ARCHIVE DESTINATION lib)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/rtc_base 
    ${PROJECT_SOURCE_DIR}/src/api
    ${PROJECT_SOURCE_DIR}/src/system_wrappers
    ${PROJECT_SOURCE_DIR}/src/modules
    ${PROJECT_SOURCE_DIR}/src/common_audio
    ${PROJECT_SOURCE_DIR}/src/common_video
    ${PROJECT_SOURCE_DIR}/src/media
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
)


