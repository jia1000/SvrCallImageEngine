
#include "common_utils.h"
#include "image_process.h"


int GetJsonDataInt(const rapidjson::Document& doc, const std::string key, int& data)
{	
	if (doc.HasMember(key.c_str())) {		
		const Value& value = doc[key.c_str()];
		if (value.IsInt())
		{
			data = value.GetInt();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}
	} else {
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	}
	return RET_STATUS_FAILURE;
}
int GetJsonDataString(const rapidjson::Document& doc, const std::string key, std::string& data)
{
	if (doc.HasMember(key.c_str())) {		
		const Value& value = doc[key.c_str()];
		if (value.IsString())
		{
			data = value.GetString();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}
	} else {
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	}
	return RET_STATUS_FAILURE;	
}

int GetJsonDataFloat(const rapidjson::Document& doc, const std::string key, float& data)
{
	if (doc.HasMember(key.c_str())) {		
		const Value& value = doc[key.c_str()];
		if (value.IsDouble())
		{
			data = value.GetDouble();
			return RET_STATUS_SUCCESS;
		} else {
			// json类型错误
			printf("json value(%s) type error\n", key.c_str());
			return RET_STATUS_JSON_VALUE_TYPE_ERROR;
		}
	} else {
		// json值未找到
		printf("json value(%s) not found\n", key.c_str());
		return RET_STATUS_JSON_NAME_NOT_FOUND;
	}
	return RET_STATUS_FAILURE;	
}
