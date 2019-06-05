cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 11)

## 依赖库的包含 ##########################################################
# DCMTK dependencies
find_package(DCMTK REQUIRED NO_MODULE)

## 依赖文件的包含 #######################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${DCMTK_INCLUDE_DIRS}/)
include_directories(./DicomEngine/)
# jni.h文件的依赖
#include_directories(/usr/local/include/jdk1.8.0_211/include/)
#include_directories(/usr/local/include/jdk1.8.0_211/include/linux/)
include_directories(/opt/jdk1.8.0_151/include/)
include_directories(/opt/jdk1.8.0_151/include/linux/)

# 添加 DicomEngine 项目文件
aux_source_directory(./DicomEngine/api 			DIRSRCSdicom_engine)
aux_source_directory(./DicomEngine/api/dicom 		DIRSRCSdicom_engine)
aux_source_directory(./DicomEngine/main/controllers 	DIRSRCSdicom_engine)

## 依赖库的链接 ##########################################################
# 
link_libraries(${DCMTK_LIBRARIES})

## 生成库 ################################################################
# 生成dicom_engine库
add_library(dicom_engine  SHARED ${DIRSRCSdicom_engine})
TARGET_LINK_LIBRARIES(dicom_engine ${DCMTK_LIBRARIES})

# 生成controller库
aux_source_directory(./EngineController  DIRSRCS_Controller)
add_library(controller  SHARED ${DIRSRCS_Controller})
# 添加第三方依赖库的路径
LINK_DIRECTORIES(../thirdparty/)
TARGET_LINK_LIBRARIES(controller  dicom_engine jsoncpp)

# 生成SvrCallImageEngine库
add_compile_options(-fPIC)
add_library(SvrCallImageEngine SHARED SvrJava/SvrCallImageEngine.cpp SvrJava/SvrCallImageEngine.h)
TARGET_LINK_LIBRARIES(SvrCallImageEngine  controller)

# 指定生成目标
#add_executable(appexe app.cpp)
#target_link_libraries (appexe  controller)


