cmake_minimum_required(VERSION 3.10)

# list the source files
set(SOURCE_FILES
	src/playground.cpp
)

# add executable target
add_executable(playground
	${SOURCE_FILES}
)

target_link_libraries(playground
	PUBLIC
		Waleed::math
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

if(WIN32)
	target_compile_definitions(playground
		PRIVATE
			OS_WINDOWS=1
	)
elseif(UNIX)
	target_compile_definitions(playground
		PRIVATE
			OS_LINUX=1
	)
endif()