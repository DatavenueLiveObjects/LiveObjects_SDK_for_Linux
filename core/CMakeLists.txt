# Define various var name
set(CORE_LIB liveobjects)
set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/mqtt_live_objects)
set(PLATFORM_PATH ${CMAKE_SOURCE_DIR}/mqtt_live_objects/platforms/linux)
set(PLATFORM_IOTSOFTBOX_PATH ${PLATFORM_PATH}/iotsoftbox-linux)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)

#Bring the headers
include_directories(${SOURCE_PATH})
include_directories(${SOURCE_PATH}/LiveObjects-iotSoftbox-mqtt-core)
include_directories(${LIB_PATH})
include_directories(${LIB_PATH}/paho.mqtt.embedded-c/MQTTPacket/src)
include_directories(${LIB_PATH}/mbedtls/include)
include_directories(${PLATFORM_PATH})
include_directories(${CMAKE_SOURCE_DIR}/mbedtls_configs)

# Bring the Config
include_directories(.)

# Make a List of all source files
file(GLOB_RECURSE ALL_SOURCE ${SOURCE_PATH}/*.c)

# Generate the library from the sources
add_library(${CORE_LIB} SHARED ${ALL_SOURCE})
target_link_libraries(${CORE_LIB} ${COMMON_LIB_LIST})
