cmake_minimum_required(VERSION 3.8)

project(xrtc-third_party)

#默认存放静态库的目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
#默认存放动态库的目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
#默认存放可执行文件的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/bin)

#只有在Windows平台才会使用duilib的库
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_subdirectory("./NIM_Duilib_Framework")
endif()