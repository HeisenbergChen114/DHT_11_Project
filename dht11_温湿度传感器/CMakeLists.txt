cmake_minimum_required(VERSION 3.10)

project(dht11_weather
    LANGUAGES C CXX
    DESCRIPTION "DHT11 Weather"
    VERSION 1.0.0
)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE C_FILE_SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

find_library(WIRINGPI_LIBRARY wiringPi)

if(NOT WIRINGPI_LIBRARY)
    message(FATAL_ERROR "wiringPi library not found") 
endif(NOT WIRINGPI_LIBRARY)



# 定义并创建动态库
add_library(dht11_weather SHARED ${C_FILE_SOURCES})

# 设置头文件路径
target_include_directories(dht11_weather PUBLIC "${PROJECT_SOURCE_DIR}/include")

# 链接wiringPi库
target_link_libraries(dht11_weather ${WIRINGPI_LIBRARY})

# 设置链接库输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 添加安装规则
install(TARGETS dht11_weather
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
)
