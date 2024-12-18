cmake_minimum_required(VERSION 3.8)

project(xrtc)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB all_src
	./*.cpp)

#引入头文件目录，包括第三方库的头文件目录
include_directories(
	${XRTC_DIR}
	${XRTC_THIRD_PARTY_DIR}/include
)

#引入库文件目录，包括第三方库的库文件目录
link_directories(
	${XRTC_THIRD_PARTY_DIR}/lib
)

#生成动态库xrtc
add_library(xrtc SHARED ${all_src})

#链接第三方库到xrtc
target_link_libraries(xrtc
					  absl_base
					  absl_string_view
					  absl_strings
					  absl_bad_optional_access
					  absl_throw_delegate
					  absl_bad_variant_access
					  winmm
					  libwebrtc
)