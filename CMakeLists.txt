cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpplox)

add_executable(${PROJECT_NAME} "src/main.cpp")
add_executable(generate_ast "utilities/GenerateAst.cpp" "utilities/GenerateAst.hpp")
add_executable(ast_printer "src/AstPrinter.cpp")

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC src)
target_link_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} src)
