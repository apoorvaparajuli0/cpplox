cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpplox)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(jlox)
add_subdirectory(clox)

target_include_directories(${PROJECT_NAME} PUBLIC jlox)
target_link_directories(${PROJECT_NAME} PRIVATE jlox)
target_link_libraries(${PROJECT_NAME} jlox)

target_include_directories(${PROJECT_NAME} PUBLIC clox)
target_link_directories(${PROJECT_NAME} PRIVATE clox)
target_link_libraries(${PROJECT_NAME} clox)
