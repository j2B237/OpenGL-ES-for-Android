cmake_minimum_required(VERSION 3.22.1)

project(native-lib)

find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)

add_library(
	matrix
	STATIC
	matrix.cpp
)

# JNI shared Library
add_library(
	native-lib
	SHARED 
	native-lib.cpp
)

# Set include headers
target_include_directories(
	native-lib
	PRIVATE
	${CMAKE_SOURCE_DIR}/include
)


# Link static lib against dynamic lib (.so)
target_link_libraries (
	native-lib
	matrix
	${glesv2-lib}
	${android-lib}
	${log-lib}
)

