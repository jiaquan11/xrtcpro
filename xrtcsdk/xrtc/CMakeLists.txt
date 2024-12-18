cmake_minimum_required(VERSION 3.8)

project(xrtc)

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

add_library(xrtc SHARED ${all_src})