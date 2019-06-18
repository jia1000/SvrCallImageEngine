cmake_minimum_required(VERSION 2.8.12)
set (CMAKE_CXX_STANDARD 11)

## 依赖库的包含 ##########################################################
# DCMTK dependencies
find_package(DCMTK REQUIRED NO_MODULE)

#---ImagingEngine依赖--------------------------------------------------------------------
# >>引用VTK/glog库
find_package(VTK 8.0 REQUIRED)
find_package(glog REQUIRED)
find_package (Threads)

vtk_module_config(VTK
  vtkCommonCore
  vtkIOImage
  vtkIOXML
  vtkInteractionWidgets
  vtkRenderingVolume${VTK_RENDERING_BACKEND}
)
# >>添加头文件include路径，也就是 gcc -I xxx 的意思，或者vs ide中添加头文件包含目录
include(${VTK_USE_FILE})
# 添加头文件目录
include_directories(
	${PROJECT_SOURCE_DIR}/include
	/usr/local/include/glog
	)

LINK_DIRECTORIES(

	/usr/lib
	/usr/local/vtk-8.0/lib

	${PROJECT_SOURCE_DIR}/lib

	)
#-----------------------------------------------------------------------


## 依赖文件的包含 #######################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${DCMTK_INCLUDE_DIRS})
include_directories(./DicomEngine/)
include_directories(./ImagingEngineLib/include)

# jni.h文件的依赖
#include_directories(/usr/local/include/jdk1.8.0_211/include/)
#include_directories(/usr/local/include/jdk1.8.0_211/include/linux/)
include_directories(/opt/jdk1.8.0_151/include/)
include_directories(/opt/jdk1.8.0_151/include/linux/)


link_directories(./thirdparty/)
# link_directories(/usr/local/lib/)

# 添加 DicomEngine 项目文件
aux_source_directory(./DicomEngine/api 			DIRSRCSdicom_engine)
aux_source_directory(./DicomEngine/api/dicom 		DIRSRCSdicom_engine)
aux_source_directory(./DicomEngine/main/controllers 	DIRSRCSdicom_engine)

## 依赖库的链接 ########################################################### 
link_libraries(${DCMTK_LIBRARIES})

## 生成库 ################################################################
# 生成dicom_engine库
add_library(dicom_engine  SHARED ${DIRSRCSdicom_engine})
TARGET_LINK_LIBRARIES(dicom_engine ${DCMTK_LIBRARIES})
TARGET_LINK_LIBRARIES(dicom_engine )

# 生成controller库
aux_source_directory(./EngineController  DIRSRCS_Controller)
add_library(controller  SHARED ${DIRSRCS_Controller})
# 添加第三方依赖库的路径
TARGET_LINK_LIBRARIES(controller ${VTK_LIBRARIES})
TARGET_LINK_LIBRARIES(controller dicom_engine)
TARGET_LINK_LIBRARIES(controller jsoncpp)
TARGET_LINK_LIBRARIES(controller ImagingEngineLib)

# 开启调试信息
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 开启所有警告
set(CMAKE_CXX_FLAGS "-Wall")

# 生成SvrCallImageEngine库
add_compile_options(-fPIC)
add_library(SvrCallImageEngine SHARED SvrJava/com_deepwise_cta_component_rebuild_SvrCallImageEngine.cpp)
TARGET_LINK_LIBRARIES(SvrCallImageEngine  controller) 

# 指定生成目标
add_executable(appexe main.cpp)
target_link_libraries (appexe  controller)


