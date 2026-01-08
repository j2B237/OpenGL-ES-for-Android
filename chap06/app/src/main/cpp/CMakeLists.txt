cmake_minimum_required(VERSION 3.22.1)

project(native-lib)


find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)

add_library(
	Matrix
	STATIC
	Matrix.cpp
)

# JNI library
add_library(
	native-lib
	SHARED
	native-lib.cpp
)


target_include_directories(
	native-lib
	PRIVATE
	${CMAKE_SOURCE_DIR}/include
)


target_link_libraries(
	native-lib
	Matrix
	${android-lib}
	${log-lib}
	${glesv2-lib}
)