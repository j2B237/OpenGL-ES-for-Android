cmake_minimum_required(VERSION 3.22.1)

project(native-lib)


find_library(log-lib log)
find_library(android-lib android)
find_library(glesv2-lib GLESv2)

# Static lib
add_library(
	Matrix
	STATIC
	Matrix.cpp
)


add_library(
	Texture
	STATIC
	Texture.cpp
)

# Compile JNI lib
add_library(
	native-lib
	SHARED
	native-lib.cpp
)


# Point to the include dir
target_include_directories(
	native-lib
	PRIVATE
	${CMAKE_SOURCE_DIR}/include
)


# Link everything together
target_link_libraries(
	native-lib
	Matrix
	Texture
	${log-lib}
	${android-lib}
	${glesv2-lib}
)