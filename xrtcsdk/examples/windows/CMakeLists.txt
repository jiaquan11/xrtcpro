cmake_minimum_required(VERSION 3.8)

project(windemo)

file(GLOB all_src
	./*.cpp)

add_executable(windemo ${all_src})