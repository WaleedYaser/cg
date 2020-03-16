cmake_minimum_required(VERSION 3.10)

add_executable(raytracer src/Raytracer.cpp)

target_link_libraries(raytracer
	PUBLIC
		Waleed::math
)

add_executable(prespective src/Prespective.cpp)

target_link_libraries(prespective
	PUBLIC
		Waleed::math
)

add_executable(pinhole src/Pinhole.cpp)

target_link_libraries(pinhole
	PUBLIC
		Waleed::math
)

add_executable(raster src/Raster.cpp)

target_link_libraries(raster
	PUBLIC
		Waleed::math
)