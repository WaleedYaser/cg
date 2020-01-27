cmake_minimum_required(VERSION 3.10)

# list the header files
set(HEADER_FILES
	include/math/Vec3.h
	include/math/Vec4.h
	include/math/Mat4.h
	include/math/Polar.h
)

# list the source files
set(SOURCE_FILES)

# unity build
set(UNITY_BUILD_FILES
	src/Build.cpp
)

# ignore source files
foreach(SRC_FILE ${SOURCE_FILES})
	set_source_files_properties(${SRC_FILE} PROPERTIES HEADER_FILE_ONLY true)
endforeach(SRC_FILE)

# add library target
add_library(math
	${HEADER_FILES}
	${SOURCE_FILES}
	${UNITY_BUILD_FILES}
)

add_library(Waleed::math ALIAS math)

# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

if(WIN32)
	target_compile_definitions(math
		PRIVATE
			OS_WINDOWS=1
	)
elseif(UNIX)
	target_compile_definitions(math
		PRIVATE
			OS_LINUX=1
	)
endif()

# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(math PUBLIC cxx_std_17)
set_target_properties(math PROPERTIES
	CXX_EXTENSIONS OFF
	DEBUG_POSTFIX d
)

# generate exports header file
include(GenerateExportHeader)
generate_export_header(math
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/math/Exports.h
)

# list include directories
target_include_directories(math
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)