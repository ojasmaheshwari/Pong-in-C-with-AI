cmake_minimum_required(VERSION 3.25.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER "C:/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/mingw64/bin/g++.exe")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_TOOLCHAIN_FILE "C:/Users/HP/Desktop/Dev/c++/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(pong VERSION 0.0.1)

file(GLOB_RECURSE SOURCES src/*.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_MAKE_PROGRAM "MinGW Makefiles")
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(IMGUI_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/include)

message(${IMGUI_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})

set(SFML_DIR C:/SFML/lib/cmake/SFML)

find_package(SFML COMPONENTS graphics window system)

target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR} ${IMGUI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} -static sfml-graphics sfml-window sfml-system -static-libgcc -static-libstdc++) 
