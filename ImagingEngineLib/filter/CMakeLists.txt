cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_FILTER_SRCS)
add_library(filter ${DIR_FILTER_SRCS})

# set debug mode
set(CMAKE_BUILD_TYPE Debug)

# 安装静态库
install(TARGETS filter
    ARCHIVE DESTINATION lib/imaging_engine
)