#include <jni.h>
#include <jsoncpp/json/json.h>
#include <string.h>
#include "SvrCallImageEngine.h"

#include "EngineController/data_transfer_control.h"

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_LoadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("LoadSeries: \njson old data : %s\n", str);
	
	std::string resource_data("");
	int ret = DataTransferController::GetInstance()->
				ParseLoadSeries(str, resource_data);
	return ret; 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_SwitchSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("SwitchSeries: \njson old data : %s\n", str);

	std::string resource_data("");
	int ret = DataTransferController::GetInstance()->
				ParseSwitchSeries(str, resource_data);

	return ret;	 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_UnloadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("UnloadSeries: \njson old data : %s\n", str);
	
	std::string resource_data("");
	int ret = DataTransferController::GetInstance()->
				ParseUnloadSeries(str, resource_data);
				
	return ret; 
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_ProcessSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("ProcessSeries: \njson old data : %s\n", str);

	std::string resource_data("");
	int ret = DataTransferController::GetInstance()->
				ParseImageOperationData(str, resource_data);

	return ret;  
}