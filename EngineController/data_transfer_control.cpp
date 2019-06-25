//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"
#include "series_data_info.h"
#include "render_facade.h"

#include "tools/logger.h"

#include <algorithm>

#include <fstream> // ifstream, ifstream::in

DataTransferController* DataTransferController::instance = nullptr;
SeriesProcessParas DataTransferController::series_process_paras;
SeriesDataInfo* DataTransferController::series_info = nullptr;

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
			arr_image_process[i] = nullptr;
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
	// initialize logger
	static bool glog_loaded = false;
	if(false == glog_loaded)
	{
		// CGLogger::InitGLog("", "/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/build/");
		glog_loaded = true;
	}
		
	try
	{
		x2struct::X::loadjson(json_data, series_process_paras, false, true);
	}catch(...)
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	
	printf("load series dicom path : %s\n", series_process_paras.dicom_path.c_str());
	//
	series_info = new SeriesDataInfo(series_process_paras.dicom_path, true);

	return RET_STATUS_SUCCESS;
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
	
	printf("switch series dicom path : %s\n", series_process_paras.dicom_path.c_str());

	DW::RenderFacade::Get()->ChangeSeries(series_process_paras.series_uid);
	
	return RET_STATUS_SUCCESS;
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
	
	printf("unload series dicom path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());

	//卸载序列时，释放资源
	if (series_info)
	{
		delete series_info;
		series_info = nullptr;
	}

	DW::RenderFacade::Get()->UnloadSeries(series_process_paras.series_uid);

	return RET_STATUS_SUCCESS;
}

int DataTransferController::ParseImageOperationData(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse imageoperation's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
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