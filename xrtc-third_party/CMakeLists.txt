cmake_minimum_required(VERSION 3.8)

project(xrtc-third_party)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#创建CMakeLists.txt编译时，会自动生成一个out目录，用于存放编译生成的文件
#如下宏变量为系统默认变量，可以直接使用，自动生成相关目录
#这里的CMAKE_BINARY_DIR在windows上默认指代的是:out\build\x64-Debug
#这里的CMAKE_SYSTEM_NAME默认指代的是:Windows，Linux，Darwin等
#在windows上默认编译生成的dll库和exe文件都会放在out\build\x64-Debug\windows\bin目录下，便于加载和运行
#因为exe文件需要加载dll文件，所以dll文件也会放在exe文件的同级目录下

#默认存放静态库的目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
#默认存放动态库的目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/lib)
#默认存放可执行文件的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}/bin)

# 设置安装目录
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_SYSTEM_NAME})

#开启abseil库的安装功能，需要指定开启安装
set(ABSL_ENABLE_INSTALL true)

add_subdirectory("./jsoncpp/source")
#add_subdirectory("./libyuv")
#add_subdirectory("./libjpeg-turbo")
#add_subdirectory("./abseil-cpp")
#add_subdirectory("./libwebrtc") #已编译好，暂时不需要再编译

#只有在Windows平台才会使用duilib的库
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	#add_subdirectory("./NIM_Duilib_Framework")  #已编译好，暂时不需要再编译
endif()