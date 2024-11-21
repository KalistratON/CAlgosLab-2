CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(Array)

SET(CMAKE_CXX_STANDARD 20)
# SET(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/cl.exe")

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

if(MSVC)
    #SET(CMAKE_CXX_FLAGS_RELEASE "/MT")
    #SET(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory ("googletest")

set_target_properties(gmock         PROPERTIES FOLDER "3rdparty")
set_target_properties(gmock_main    PROPERTIES FOLDER "3rdparty")
set_target_properties(gtest         PROPERTIES FOLDER "3rdparty")
set_target_properties(gtest_main    PROPERTIES FOLDER "3rdparty")

file(GLOB SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

ADD_EXECUTABLE(Array ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Array PRIVATE gtest gmock)

target_include_directories(Array PRIVATE ${gtest_SOURCE_DIR}/include)
target_include_directories(Array PRIVATE ${gmock_SOURCE_DIR}/include)