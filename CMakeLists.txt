cmake_minimum_required(VERSION 3.25.1)
project(MQTT_PROJ LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nanopb/extra)
find_package(Nanopb REQUIRED)
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  SET(requiredlibs ${requiredlibs} ${CURL_LIBRARIES} )
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)
include_directories(${NANOPB_INCLUDE_DIRS})

set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(BUILD_OBJECT_LIBS OFF CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp)

nanopb_generate_cpp(PROTO_SRC PROTO_HDRS cs-protobuf/mqttpkt.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})  


set(MQTT_PROG_SRC src/mqtt_cli.cpp)
set(MQTT_PROG_INCLUDE include include/post include/mqtt_cli)

add_executable(mqtt_prog src/post.cpp ${MQTT_PROG_SRC} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(mqtt_prog PRIVATE ${CURL_LIBRARY} jsoncpp_static)
target_include_directories(mqtt_prog PRIVATE ${MQTT_PROG_INCLUDE})



