cmake_minimum_required(VERSION 3.12)
project(ED_Lib)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
    add_compile_options(/bigobj /MP /utf-8)
endif()

find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(EDLib STATIC
        src/ED.cpp
        include/ED.h
        src/EDCircles.cpp
        include/EDCircles.h
        src/EDColor.cpp
        include/EDColor.h
        include/EDLib.h
        src/EDLines.cpp
        include/EDLines.h
        src/EDPF.cpp
        include/EDPF.h
        src/NFA.cpp
        include/NFA.h)
target_include_directories(EDLib PUBLIC include)
target_link_libraries(EDLib PUBLIC ${OpenCV_LIBS})

add_executable(test example/test.cpp)
target_link_libraries(test PRIVATE EDLib)

add_executable(test_it example/test_it.cpp)
target_link_libraries(test_it PRIVATE EDLib)