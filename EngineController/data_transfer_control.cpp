//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

#include <algorithm>


//#include "json/json.h"


#include <fstream> // ifstream, ifstream::in
//#include <io.h>
#include <chrono>


//#include "rapidjson/rapidjson.h"
//#include "rapidjson/document.h"
//#include "rapidjson/reader.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"
//
//using namespace rapidjson;


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
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
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
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
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
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
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
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Document doc;
	doc.Parse(json_data);

	std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
	std::chrono::duration<double> span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	static std::chrono::duration<double> total_diff = std::chrono::duration<double>(0);;
	total_diff += span;

	// 获得关键性的参数
	int key_name1 = 0;
	std::string key_name2("");
	std::string key_name3("");
	std::string key_name4("");
	//std::string key_name5("");
	//std::string key_name6("");
	int key_name5 = 0;
	int key_name6 = 0;

	if (doc.HasMember(JSON_KEY_IMAGE_TYPE)) {
		const Value& value = doc[JSON_KEY_IMAGE_TYPE];
		key_name1 = value.GetInt();
	}
	//if (doc.HasMember(JSON_KEY_VESSEL_NAME)) {
	//	const Value& value = doc[JSON_KEY_VESSEL_NAME];
	//	key_name2 = value.GetString();
	//	// 都转为大写字符
	//	std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::toupper);
	//}


	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		//CGLogger::Debug("Debug_CTA:VR---begin");
		image_process = new ImageVRProcess(key_name3);
		image_process->SetDocument(json_data);
		//image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		//CGLogger::Debug("Debug_CTA:VR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		//CGLogger::Debug("Debug_CTA:MPR---begin");
		printf("Debug_CTA:MPR---begin\n");
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetDocument(json_data);
		//image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		//CGLogger::Debug("Debug_CTA:MPR---");
		printf("Debug_CTA:MPR---end\n");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {
		image_process = new ImageCPRProcess(key_name3);
		image_process->SetDocument(json_data);
		//image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);		
	} else {//if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {	
		printf("switch time..\n");	
		return true;
	} 

	// 模拟再发送给浏览器
	

	return true;
}

