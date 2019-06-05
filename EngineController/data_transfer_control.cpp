//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"

#include <algorithm>

//#include "json/json.h"

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

int DataTransferController::ParseLoadSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);	

	std::string data("");
	int ret = GetJsonDataString(doc, JSON_KEY_DICOM_PATH, data);
	printf("load series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseSwitchSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);
	
	std::string data("");
	int ret = GetJsonDataString(doc, JSON_KEY_DICOM_PATH, data);
	printf("switch series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseUnloadSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);
	
	std::string data("");
	int ret = GetJsonDataString(doc, JSON_KEY_DICOM_PATH, data);
	printf("unload series dicom path : %s\n", data.c_str());

	return ret;
}

int DataTransferController::ParseImageOperationDataUseRapidJson(const char* json_data, std::string& js_data)
{
	std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	static std::chrono::duration<double> total_diff = std::chrono::duration<double>(0);;
	total_diff += span;

	// ��ùؼ��ԵĲ���
	int key_name1 = 0;
	std::string key_name3("");

	int ret = GetJsonDataInt(doc, JSON_KEY_DICOM_PATH, key_name1);

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

	// ģ���ٷ��͸������	

	return true;
}

