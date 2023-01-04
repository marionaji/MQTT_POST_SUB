cmake_minimum_required(VERSION 3.23.1)
project(MQTT_PROJ LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

nanopb_generate_cpp(PROTO_SRC PROTO_HDRS cs-protobuf/arduino_mqtt_pkt.proto cs-protobuf/jetson_mqttpkt.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})  

set(MQTT_PROG_SRC src/mqtt_cli.cpp)
set(MQTT_PROG_INCLUDE include)

add_executable(mqtt_prog src/post.cpp ${MQTT_PROG_SRC} ${PROTO_SRC} ${PROTO_HDRS} src/util.cpp src/pb_parse.cpp src/mqtt_callback.cpp)
target_link_libraries(mqtt_prog mosquitto)
target_include_directories(mqtt_prog PRIVATE ${MQTT_PROG_INCLUDE})

# Unit Testing
include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)