//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"

#include "tools/logger.h"

#include <algorithm>

#include <fstream> // ifstream, ifstream::in
#include <chrono>

DataTransferController* DataTransferController::instance = nullptr;
SeriesProcessParas DataTransferController::series_process_paras;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{		
		switch (i)
		{
		case JSON_VALUE_REQUEST_TYPE_VR:
			arr_image_process[i] = new ImageVRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_MPR:
			arr_image_process[i] = new ImageMPRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_CPR:
			arr_image_process[i] = new ImageCPRProcess();
			break;
		default:
			break;
		}				
	}	
}


DataTransferController::~DataTransferController()
{
	instance = nullptr;

	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{
		if (arr_image_process[i])
		{
			delete arr_image_process[i];
			arr_image_process[i] = nullptr;
		}		
	}	
}

int DataTransferController::ParseLoadSeries(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse loadserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	// initialize logger
	CGLogger::InitGLog("", "/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/build/");
	
	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.dicom_path);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.mask_path);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.curve_path);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.patient_id);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.study_uid);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.series_uid);

	printf("load series dicom path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());

	return ret;
}

int DataTransferController::ParseSwitchSeries(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse switchserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.dicom_path);
	if(ret <= 0) return ret;
	printf("switch series dicom path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());

	return ret;
}

int DataTransferController::ParseUnloadSeries(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse unloadserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, DataTransferController::series_process_paras.dicom_path);
	if(ret <= 0) return ret;
	printf("unload series dicom path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());

	return ret;
}

int DataTransferController::ParseImageOperationData(const char* json_data)
{
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse imageoperation's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	static std::chrono::duration<double> total_diff = std::chrono::duration<double>(0);;
	total_diff += span;

	// 获得关键性的参数
	int request_type = 0;
	int ret = GetJsonDataInt(root, JSON_KEY_IMAGE_TYPE, request_type);
	
	if (ret <= 0)
	{
		return ret;
	}		

	if (request_type >= 0 && request_type < JSON_VALUE_REQUEST_TYPE_MAX)
	{
		if (arr_image_process[request_type])
		{
			arr_image_process[request_type]->Excute(json_data);
		}		
	}	

	return true;
}