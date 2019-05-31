//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"

#include <algorithm>


//#include "json/json.h"


#include <fstream> // ifstream, ifstream::in
//#include <io.h>
#include <chrono>


#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;


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
	std::string key_name1("");
	std::string key_name2("");
	std::string key_name3("");
	std::string key_name4("");
	//std::string key_name5("");
	//std::string key_name6("");
	int key_name5 = 0;
	int key_name6 = 0;

	if (doc.HasMember(JSON_KEY_REQUEST_TYPE)) {
		const Value& value = doc[JSON_KEY_REQUEST_TYPE];
		key_name1 = value.GetString();
		// 都转为小写字符
		std::transform(key_name1.begin(), key_name1.end(), key_name1.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_OPERATION)) {
		const Value& value = doc[JSON_KEY_IMAGE_OPERATION];
		key_name2 = value.GetString();
		// 都转为小写字符
		std::transform(key_name2.begin(), key_name2.end(), key_name2.begin(), ::tolower);
	}

	if (doc.HasMember(JSON_KEY_IMAGE_PARAS)) {
		const Value& value = doc[JSON_KEY_IMAGE_PARAS];
		key_name3 = value.GetString();
		// 都转为小写字符
		std::transform(key_name3.begin(), key_name3.end(), key_name3.begin(), ::tolower);
	}
	if (doc.HasMember(JSON_KEY_IMAGE_DATA)) {
		const Value& value = doc[JSON_KEY_IMAGE_DATA];
		key_name4 = value.GetString();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_SEQUENCE)) {
		const Value& value = doc[JSON_KEY_IMAGE_SEQUENCE];
		key_name5 = value.GetInt();
	}
	if (doc.HasMember(JSON_KEY_IMAGE_MAX)) {
		const Value& value = doc[JSON_KEY_IMAGE_MAX];
		key_name6 = value.GetInt();
	}

	if(image_process) {
		delete image_process;
		image_process = NULL;
	}

	std::string out_image_data = "";

	if (key_name1 == JSON_VALUE_REQUEST_TYPE_MPR) {
		//CGLogger::Debug("Debug_CTA:MPR---begin");
		printf("Debug_CTA:MPR---begin\n");
		image_process = new ImageMPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		//CGLogger::Debug("Debug_CTA:MPR---");
		printf("Debug_CTA:MPR---end\n");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_VR) {
		//CGLogger::Debug("Debug_CTA:VR---begin");
		image_process = new ImageVRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);
		//CGLogger::Debug("Debug_CTA:VR---");
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_CPR) {
		image_process = new ImageCPRProcess(key_name3);
		image_process->SetKey2_ImageOperation(key_name2);
		image_process->Excute(out_image_data);		
	} else if (key_name1 == JSON_VALUE_REQUEST_TYPE_TIME) {	
		printf("switch time..\n");	
		return true;
	} 

	// 模拟再发送给浏览器
	

	return true;
}

