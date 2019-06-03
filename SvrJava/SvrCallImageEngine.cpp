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
	if (DataTransferController::GetInstance()->ParseLoadSeriesUseRapidJson(
	str, resource_data)) {
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
	if (DataTransferController::GetInstance()->ParseSwitchSeriesUseRapidJson(
	str, resource_data)) {
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
	if (DataTransferController::GetInstance()->ParseUnloadSeriesUseRapidJson(
	str, resource_data)) {
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
	if (DataTransferController::GetInstance()->ParseImageOperationDataUseRapidJson(
	str, resource_data)) {
		return RET_STATUS_FAILURE;
	}
	return RET_STATUS_SUCCESS;  
}

JNIEXPORT jstring JNICALL Java_SvrCallImageEngine_test_1json
  (JNIEnv *env, jobject obj , jstring string)
{
	

#if 0
	{
		std::string key = "request_type";
		Value& s = d[key.c_str()];
		s.SetString("mpr");
		printf("%s : %s\n", key.c_str(), s.GetString());
	}
	{
		std::string key = "image_operation";
		Value& s = d[key.c_str()];
		s.SetString("Move");
		printf("%s : %s\n", key.c_str(), s.GetString());
	}
	{
		std::string key = "image_paras";
		Value& s = d[key.c_str()];
		s.SetString("0.9");
		printf("%s : %s\n", key.c_str(), s.GetString());
	}
	
	StringBuffer buff;
	Writer<StringBuffer> writer(buff);
	d.Accept(writer);

	printf("json new data : %s\n", buff.GetString());
#endif
	///////////////////////////////
	//Test_Controller("../10.dcm", "../20.dcm");
	///////////////////////////////

	char cap[128] = {0};  
	return env->NewStringUTF(cap);  
}

JNIEXPORT void JNICALL Java_SvrCallImageEngine_print
  (JNIEnv *env, jobject obj)
{
	printf("SvrCallImageEngine JNI!\n");
	return;
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_test_1int1
  (JNIEnv *env, jobject obj, jint n)
{
	int ret = n * n;
	printf("test_int1(%d)  :  ret = %d\n", n, ret);
	return ret;
}

JNIEXPORT jfloat JNICALL Java_SvrCallImageEngine_test_1double1
  (JNIEnv *env, jobject obj, jdouble d)
{
	double ret = d + 20.0;
	printf("test_float1(%.2f)  :  ret = %.2f\n", d, ret);
	return ret;
}

JNIEXPORT jstring JNICALL Java_SvrCallImageEngine_test_1string1
  (JNIEnv *env, jobject obj, jstring string)
{
#ifdef __cplusplus
	const char* str = env->GetStringUTFChars(string, 0);  
	char cap[128] = {0};  
    	strcpy(cap, str);  
    	env->ReleaseStringUTFChars(string, 0);
	strcat(cap, " from JNI cpp!");

	printf("test_string1(%s) : %s\n", str, cap);
    	//return env->NewStringUTF(" Hello from JNI cpp!");  
	return env->NewStringUTF(cap);  
#else
	//const char* str = (*env)->GetStringUTFChars(env, string, 0);  
    	//char cap[128];  
    	//strcpy(cap, str);  
    	//(*env)->ReleaseStringUTFChars(env, string, 0); 
	//cap[0] = 'A'; 
    	//return (*env)->NewStringUTF(env, cap);  
	//return (*env)->NewStringUTF(env, std::strupr(cap));
	return (*env)->NewStringUTF(env, (char *)"Hello from JNI c!");  
#endif
}

JNIEXPORT jint JNICALL Java_SvrCallImageEngine_test_1int_1array
  (JNIEnv *env, jobject obj, jintArray array)
{
	int i, sum = 0;  
    	jsize len = env->GetArrayLength(array);  
    	jint *body = env->GetIntArrayElements(array, 0);  
       
    	for (i = 0; i < len; ++i)  
    	{  
        	sum += body[i];  
    	}  
    	env->ReleaseIntArrayElements(array, body, 0);  

	printf("test_int_array()  :  sum = %d\n", sum);
    	return sum;  
}


