cmake_minimum_required(VERSION 3.8)

project(xrtcsdk)

#设置C++标准为C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#根据系统位数和编译类型，设置不同的编译目录
if (CMAKE_CL_64) 
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(BUILD_TYPE "x64-Debug")
    else()
        set(BUILD_TYPE "x64-Release")
    endif()
else()
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(BUILD_TYPE "x86-Debug")
    else()
        set(BUILD_TYPE "x86-Release")
    endif()
endif()

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

#设置XRTC_DIR为当前目录
set(XRTC_DIR ${CMAKE_CURRENT_SOURCE_DIR}) 

#设置XRTC_THIRD_PARTY_DIR为第三方库的目录
set(XRTC_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../xrtc-third_party/out/install/${BUILD_TYPE}/${CMAKE_SYSTEM_NAME})

#解决windows下编译时，出现的一些警告，实参与形参类型不匹配的问题
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	add_definitions(-DUNICODE -D_UNICODE)
endif()

#添加编译的子模块
add_subdirectory("./xrtc")
add_subdirectory("./examples")