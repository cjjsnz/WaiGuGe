cmake_minimum_required(VERSION 2.8.12)
project (main)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS"${CMAKE_C_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS} -O0 -g")



set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_target_properties(${TARGET} PROPERTIES
COMPILE_FLAGS "-pthread"
LINK_FLAGS "-pthread")
include_directories(./)

SET(SRC
main.cpp
ADS1263.cpp
DEV_Config.cpp
bcm2835.c
EMGFilters.cpp
kalman_Filter.cpp
EMGData.cpp
Motor.cpp
MotorControl.cpp
initial.cpp
keyboard.cpp

)

add_executable(${PROJECT_NAME} ${SRC} )

target_link_libraries(${PROJECT_NAME} -pthread)

target_link_libraries(${PROJECT_NAME} -lEposCmd )

target_link_libraries(${PROJECT_NAME} -lrt)

