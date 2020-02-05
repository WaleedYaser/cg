cmake_minimum_required(VERSION 3.10)

add_executable(raytracer src/Raytracer.cpp)

target_link_libraries(raytracer
	PUBLIC
		Waleed::math
)