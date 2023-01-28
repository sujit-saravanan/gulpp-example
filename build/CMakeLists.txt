cmake_minimum_required(VERSION 3.23)
project(test)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS  "-lglfw")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../output)


add_library(khrplatform STATIC ../modules/khrplatform/khrplatform.cpp)
target_include_directories(khrplatform PUBLIC ../modules/khrplatform/include)

add_library(gulpp STATIC ../modules/gulpp/gulpp.cpp)
target_include_directories(gulpp PUBLIC ../modules/gulpp/include)

add_library(glad STATIC ../modules/glad/glad.cpp)
target_include_directories(glad PUBLIC ../modules/glad/include)

add_library(stb_image STATIC ../modules/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC ../modules/stb_image/include)



add_executable(${PROJECT_NAME} ../source/main.cpp)

target_link_libraries(gulpp PUBLIC glad)

target_link_libraries(glad PUBLIC khrplatform)

target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)
target_link_libraries(${PROJECT_NAME} PUBLIC gulpp)

