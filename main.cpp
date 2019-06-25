

#include "EngineController/data_transfer_control.cpp"

#include <string.h>
#include <iostream>
// using namespace std;

int loadSeries(std::string str);
int switchSeries(std::string str);
int unloadSeries(std::string str);
int processSeries(std::string str);

class DataTransferController;

int main(int argc, char** args)
{	
	std::printf("begin debug.\n");
	bool opened = true;
	const int MAX_TEST = 2;
		std::string series_uid[MAX_TEST] = {
			"\"series_uid\":\"1\"",
			"\"series_uid\":\"2\""
		};
	for (int i = 0; i < MAX_TEST - 0; i++)
	{
		// 加载Series
		if(opened)
		{
			std::string dicom_path[MAX_TEST] = {
					"\"dicom_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/\",",
					"\"dicom_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source2/\","
				};
			std::string json_data = "{";
			json_data += dicom_path[i];//"\"dicom_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/\",";
			json_data += "\"mask_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/mask_source/\",";
			json_data += "\"curve_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/curve_source/\",";
			json_data += "\"patient_id\":\"zhangsan\",";
			json_data += "\"study_uid\":\"111\",";
			json_data += series_uid[i];//"\"series_uid\":\"222\"";
			json_data += "}";
			loadSeries(json_data);
		}
		// 切换Series
		if(opened)
		{			
			std::string json_data = "{";
			json_data += "\"patient_id\":\"lilsi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += series_uid[i];//"\"series_uid\":\"444\"";
			json_data += "}";
			switchSeries(json_data);
		}
		if(opened)
		{
			std::string output_path[MAX_TEST] = {
				"\"output_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/build/vr1/\",",
				"\"output_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/build/vr2/\","
			};
			std::string output_image_number[MAX_TEST] = {
				"\"output_image_number\":12,",
				"\"output_image_number\":16,"
			};
			
			// VR 和 VRMIP 操作
			std::string json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":0,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":0,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目c
			json_data += output_path[i];//"\"output_path\":\"/home/My_Demo_Test/SvrCallImageEngineGit/SvrCallImageEngine/build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		if(!opened)
		{
			// MPRMIP 操作
			std::string json_data = "{";
			json_data += "\"image_type\":1,"; 			// 图像类型
			json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":0,"; 	// 正位（起始位置）标识
			json_data += "\"clip_percent\":0.35,"; 		// 截取比例;  
			json_data += "\"thickness\":25.0,"; 		// 层厚
			json_data += "\"spacing_between_slices\":5.0,";// 层间距
			json_data += "\"output_path\":\"../build/mprmip1/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":999"; 		// 
			json_data += "}";
			processSeries(json_data);
		}
		// 卸载Series
		if(opened)
		{
			std::string json_data = "{";
			json_data += "\"patient_id\":\"wangmazi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += series_uid[i];//"\"series_uid\":\"666\"";
			json_data += "}";
			unloadSeries(json_data);
		}
	}
	std::printf("end debug.\n");

	return 0;
}

int loadSeries(std::string str)
{
	std::printf("LoadSeries: \njson old data : %s\n", str.c_str());
	
	int ret = DataTransferController::GetInstance()->ParseLoadSeries(str.c_str());
	return ret; 
}

int switchSeries(std::string str)
{
	std::printf("switchSeries: \njson old data : %s\n", str.c_str());
	
	int ret = DataTransferController::GetInstance()->ParseSwitchSeries(str.c_str());
	return ret; 
}

int unloadSeries(std::string str)
{
	std::printf("unloadSerirs: \njson old data : %s\n", str.c_str());
	
	int ret = DataTransferController::GetInstance()->ParseUnloadSeries(str.c_str());
	return ret; 
}

int processSeries(std::string str)
{
	std::printf("ProcessSeries: \njson old data : %s\n", str.c_str());

	int ret = DataTransferController::GetInstance()->ParseImageOperationData(str.c_str());

	return ret;  
}
