
#include "common_utils.h"
#include "image_process.h"


int GetJsonDataInt(const Json::Value& root, const std::string key, int& data)
{	
	const Json::Value& value = root[key];
	if (value.isNull()) {
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;		
	} else {
		if (value.isInt())
		{
			data = value.asInt();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}
	}
	return RET_STATUS_FAILURE;
}

int GetJsonDataString(const Json::Value& root, const std::string key, std::string& data)
{
	const Json::Value& value = root[key];
	if (value.isNull()) {		
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	} else {
		if (value.isString()) {
			data = value.asString();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}		
	}
	return RET_STATUS_FAILURE;	
}

int GetJsonDataFloat(const Json::Value& root, const std::string key, float& data)
{
	const Json::Value& value = root[key];
	if (value.isNull()) {		
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	} else {
		if (value.isDouble())
		{
			data = value.asDouble();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}		
	}
	return RET_STATUS_FAILURE;	
}
