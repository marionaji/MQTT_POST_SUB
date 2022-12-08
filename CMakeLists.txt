cmake_minimum_required(VERSION 3.25.1)
project(MQTT_PROJ LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

nanopb_generate_cpp(PROTO_SRC PROTO_HDRS cs-protobuf/mqttpkt.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})  
set(MQTT_PROG_INCLUDE include include/post)

add_executable(mqtt_prog src/post.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(mqtt_prog PRIVATE ${MQTT_PROG_INCLUDE})