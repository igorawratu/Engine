project(engine)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/FindSDL2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#check dependencies
find_package(SDL2 REQUIRED)

#Includes
include_directories( ${PROJECT_NAME}
    ${SDL2_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
