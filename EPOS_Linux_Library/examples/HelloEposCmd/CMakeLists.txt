cmake_minimum_required(VERSION 2.8.12)
project (HelloEposCmd)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(./)

include_directories(/home/pi/bcm2835-1.71/src)

SET(SRC
HelloEposCmd.cpp 
initial.cpp 
server.cpp 
tes.cpp 
emg_adc.c 
ADS1263.c
DEV_Config.c
)
add_executable(${PROJECT_NAME} ${SRC} )

target_link_libraries(${PROJECT_NAME} -lEposCmd )
target_link_libraries(${PROJECT_NAME} -lbcm2835)

