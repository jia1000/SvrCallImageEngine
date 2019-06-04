//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

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

bool DataTransferController::ParseLoadSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);
	
	const char JSON_KEY_DICOM_PATH[] = "dicom_path";
	if (doc.HasMember(JSON_KEY_DICOM_PATH)) {
		const Value& value = doc[JSON_KEY_DICOM_PATH];
		printf("load series dicom path : %s\n", value.GetString());
		
	}

	return true;
}

bool DataTransferController::ParseSwitchSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);
	
	const char JSON_KEY_DICOM_PATH[] = "dicom_path";
	if (doc.HasMember(JSON_KEY_DICOM_PATH)) {
		const Value& value = doc[JSON_KEY_DICOM_PATH];
		printf("switch series dicom path : %s\n", value.GetString());
		
	}

	return true;
}

bool DataTransferController::ParseUnloadSeriesUseRapidJson(const char* json_data, std::string& js_data)
{
	// ��������������͹�����Json����  //json�ֶν���Ҫ�������жϡ�
	Document doc;
	doc.Parse(json_data);
	
	const char JSON_KEY_DICOM_PATH[] = "dicom_path";
	if (doc.HasMember(JSON_KEY_DICOM_PATH)) {
		const Value& value = doc[JSON_KEY_DICOM_PATH];
		printf("unload series dicom path : %s\n", value.GetString());
		
	}

	return true;
}

bool DataTransferController::ParseImageOperationDataUseRapidJson(const char* json_data, std::string& js_data)
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

	if (doc.HasMember(JSON_KEY_IMAGE_TYPE)) {
		const Value& value = doc[JSON_KEY_IMAGE_TYPE];
		key_name1 = value.GetInt();
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

