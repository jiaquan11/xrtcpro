cmake_minimum_required(VERSION 3.8)

project(windemo)

#设置头文件目录
include_directories(
	${XRTC_DIR}
	${XRTC_DIR}/../xrtc-third_party/NIM_Duilib_Framework
)

#设置库文件目录
link_directories(
	${XRTC_DIR}/../xrtc-third_party/out/build/x64-Debug/Windows/lib
)

#指定需要编译的源文件
file(GLOB all_src
	./src/*.cpp
)

#生成可执行文件
add_executable(windemo ${all_src})


#链接第三方库
target_link_libraries(windemo base duilib Imm32 comctl32)
#表示设置一个windows窗口程序，而不是控制台程序
set_target_properties(windemo PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO")

#拷贝资源文件，这样运行时就可以找到资源文件，实时更新
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})