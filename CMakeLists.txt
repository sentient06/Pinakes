cmake_minimum_required(VERSION 2.8)

project(Pinax)

# Add library path

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/LFiles/include
)
link_directories(${CMAKE_SOURCE_DIR}/libs/LFiles/lib)

add_application(Pinax
	main.cpp
	main.r
	src/PIntro.cpp
	src/PMenu.cpp
	CREATOR "PinX"
)

# Link the library
# target_link_libraries(Pinax LFiles)

target_link_libraries(Pinax 
	${CMAKE_SOURCE_DIR}/libs/LFiles/lib/libLFiles.a
)