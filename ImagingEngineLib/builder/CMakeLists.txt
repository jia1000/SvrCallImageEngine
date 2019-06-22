cmake_minimum_required(VERSION 2.8)

aux_source_directory(. DIR_BUILDER_SRCS)
add_library(builder ${DIR_BUILDER_SRCS})
