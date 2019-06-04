#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "SvrCallImageEngine.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//#include "EngineController/Controller.h"
#include "EngineController/data_transfer_control.h"

using namespace rapidjson;

#define  RET_STATUS_FAILURE    (-1)
#define  RET_STATUS_SUCCESS    (1)


JNIEXPORT jint JNICALL Java_SvrCallImageEngine_LoadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("LoadSeries: \n\njson old data : %s\n", str);

	Document d;
  d.Parse(str);

	std::string resource_data("");
	if (DataTransferController::GetInstance()->
				ParseLoadSeriesUseRapidJson(str, resource_data)) {
		return RET_STATUS_FAILURE;
	}
	return RET_STATUS_SUCCESS; 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_SwitchSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("SwitchSeries: \n\njson old data : %s\n", str);

	Document d;
  d.Parse(str);

	std::string resource_data("");
	if (DataTransferController::GetInstance()->
				ParseSwitchSeriesUseRapidJson(str, resource_data)) {
		return RET_STATUS_FAILURE;
	}
	
	return RET_STATUS_SUCCESS; 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_UnloadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("UnloadSeries: \n\njson old data : %s\n", str);

	Document d;
  d.Parse(str);

	std::string resource_data("");
	if (DataTransferController::GetInstance()->
				ParseUnloadSeriesUseRapidJson(str, resource_data)) {
		return RET_STATUS_FAILURE;
	}
	return RET_STATUS_SUCCESS; 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_ProcessSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("ProcessSeries: \n\njson old data : %s\n", str);

	Document d;
  d.Parse(str);

	std::string resource_data("");
	if (DataTransferController::GetInstance()->
				ParseImageOperationDataUseRapidJson(str, resource_data)) {
		return RET_STATUS_FAILURE;
	}
	return RET_STATUS_SUCCESS;  
}