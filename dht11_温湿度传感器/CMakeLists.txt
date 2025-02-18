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


# 这份 CMakeLists.txt 的构建逻辑顺序如下：
# 设置 CMake 的最低版本要求为 3.10。
# 定义项目 dht11_weather，指定支持的语言、项目描述和版本。
# 设置 C++ 的标准为 17。
# 查找 wiringPi 库，如果未找到则报错终止。
# 遍历指定目录下的 C 语言源文件。
# 创建名为 dht11_weather 的动态库。
# 设置动态库的头文件包含路径。
# 将 wiringPi 库链接到动态库。
# 设置动态库的输出路径。
# 添加动态库的安装规则，指定安装到 lib 目录。

# 出错在没有找动态库wiringPi同时链接wiringPi