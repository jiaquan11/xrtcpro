cmake_minimum_required(VERSION 3.8)

project(xrtc)

file(GLOB all_src
	./*.cpp)

include_directories(${XRTC_DIR})

add_library(xrtc SHARED ${all_src})