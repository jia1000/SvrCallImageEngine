#include <jni.h>
#include <jsoncpp/json/json.h>
#include <string.h>
#include "com_deepwise_cta_component_rebuild_SvrCallImageEngine.h"

#include "EngineController/data_transfer_control.h"

JNIEXPORT jint JNICALL Java_com_deepwise_cta_component_rebuild_SvrCallImageEngine_loadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("LoadSeries: \njson old data : %s\n", str);
	
	int ret = DataTransferController::GetInstance()->ParseLoadSeries(str);
	return ret; 
}

JNIEXPORT jint JNICALL Java_com_deepwise_cta_component_rebuild_SvrCallImageEngine_switchSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("SwitchSeries: \njson old data : %s\n", str);

	int ret = DataTransferController::GetInstance()->ParseSwitchSeries(str);

	return ret;	 
}

JNIEXPORT jint JNICALL Java_com_deepwise_cta_component_rebuild_SvrCallImageEngine_unloadSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("UnloadSeries: \njson old data : %s\n", str);
	
	int ret = DataTransferController::GetInstance()->ParseUnloadSeries(str);
				
	return ret; 
}

JNIEXPORT jint JNICALL Java_com_deepwise_cta_component_rebuild_SvrCallImageEngine_processSeries
  (JNIEnv *env, jobject obj , jstring string)
{
	const char* str = env->GetStringUTFChars(string, 0);  
	printf("ProcessSeries: \njson old data : %s\n", str);

	int ret = DataTransferController::GetInstance()->ParseImageOperationData(str);

	return ret;  
}