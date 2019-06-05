//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"

#include <algorithm>

#include <fstream> // ifstream, ifstream::in
//#include <io.h>
#include <chrono>

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
{
	image_process = NULL;
}


DataTransferController::~DataTransferController()
{
	instance = nullptr;
}

int DataTransferController::ParseLoadSeries(const char* json_data, std::string& js_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	printf("ret 01 \n");

	Json::Value root;
	Json::Reader reader;

	printf("ret 1 \n");

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse loadserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	printf("ret 2 \n");
	std::string data("");
	int ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, data);
	printf("load series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseSwitchSeries(const char* json_data, std::string& js_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse switchserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	std::string data("");
	int ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, data);
	printf("switch series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseUnloadSeries(const char* json_data, std::string& js_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse unloadserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	std::string data("");
	int ret = GetJsonDataString(root, JSON_KEY_DICOM_PATH, data);
	printf("unload series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseImageOperationData(const char* json_data, std::string& js_data)
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
	int key_name1 = 0;
	std::string key_name3("");

	int ret = GetJsonDataInt(root, JSON_KEY_IMAGE_TYPE, key_name1);
	
	if (ret <= 0)
	{
		return ret;
	}	

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		image_process = new ImageVRProcess(key_name3);
		image_process->SetDocument(json_data);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetDocument(json_data);
		image_process->Excute(out_image_data);
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {
		image_process = new ImageCPRProcess(key_name3);
		image_process->SetDocument(json_data);
		image_process->Excute(out_image_data);		
	} else {	
		printf("switch other..\n");	
		return true;
	} 

	// 模拟再发送给浏览器	

	return true;
}

