/*=========================================================================

  Program:   ImagingEngine
  Module:    data_definition.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

// Windows 头文件: 
#ifdef WIN32
#include <windows.h>
#else

#endif
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <stdint.h>
#include <string>
#include "tools/logger.h"

#define UNITDATA3D			unsigned char
#define UNITMARK3D			unsigned char
#define UNITDATASHOW		char
#define MAXIMIUM_CT_VALUE	4096
#define MAXIMIUM_THREADS	16

#ifdef WIN32

#else
typedef unsigned char	BYTE;
typedef int				BOOL;
typedef unsigned long	DWORD;
typedef unsigned short	WORD;
#endif

// Error code
#define RET_STATUS_IE_SUCCESS							1
#define RET_STATUS_IE_FAILURE							0
#define RET_STATUS_IE_NOT_ENOUGH_MEMORY					1
#define RET_STATUS_IE_IO_DISK_CANNOT_WRITE				1
#define RET_STATUS_IE_IO_DISK_NO_SPACE					1
#define RET_STATUS_IE_IO_FILE_EXIST						1
#define RET_STATUS_IE_INPUT_INVALID_PARAM				1
#define RET_STATUS_IE_DATA_SMALL_IMAGE_NUMBER			1
#define RET_STATUS_IE_DATA_NOT_CT_IMAGE					1
#define RET_STATUS_IE_DATA_NOT_SUPPORTED_CT_IMAGE		1
#define RET_STATUS_IE_DATA_DIFFERENCT_SPACING			1
#define RET_STATUS_IE_DATA_NOT_CONTINUOUS_POSITION		1
#define RET_STATUS_IE_LIB_NOT_FOUND						1
#define RET_STATUS_IE_LIB_WRONG_VERSION					1
#define RET_STATUS_IE_EXCEPTION_NULL_POINTER			1
#define RET_STATUS_IE_EXCEPTION_INVALID_POINTER			1
#define RET_STATUS_IE_EXCEPTION_READONLY				1
#define RET_STATUS_IE_EXCEPTION_NO_DATA					1


using namespace std;

#define PI 3.14159265f

//typedef enum dwRenderMode {
//	Slice = 0,
//	Slab,
//	MPR,
//	VR,
//	CPR
//} RenderMode;

struct Point3d {
	float x;
	float y;
	float z;
};

struct Point3f {
	float x;
	float y;
	float z;
};

struct Point3i {
	int x;
	int y;
	int z;
};

struct Point2 {
	int x;
	int y;
};