cmake_minimum_required(VERSION 3.25.1)
project(MQTT_PROJ LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS} )

nanopb_generate_cpp(PROTO_SRC PROTO_HDRS RELPATH cs-protobuf cs-protobuf/arduino_mqtt_pkt.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})  

set(MQTT_PROG_SRC src/mqtt_cli.cpp src/mqtt_packet.cpp)
# set(MQTT_PROG_INCLUDE include include/post include/mqtt_cli include/mqtt_packet)
set(MQTT_PROG_INCLUDE include)
set(MQTT_DLL_FILES mosquitto.dll mosquitto_dynamic_security.dll mosquittopp.dll libssl-1_1-x64.dll libcrypto-1_1-x64.dll)

add_executable(mqtt_prog src/post.cpp ${MQTT_PROG_SRC} ${PROTO_SRC} ${PROTO_HDRS} ${NANOPB_SRCS} ${NANOPB_HDRS})
find_library(NEW_LIB mosquitto PATHS ${CMAKE_CURRENT_SOURCE_DIR}/devel)
target_link_libraries(mqtt_prog ${NEW_LIB})
target_include_directories(mqtt_prog PRIVATE ${MQTT_PROG_INCLUDE} devel)
file (COPY ${MQTT_DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})