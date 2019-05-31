
/****************************************************************************************************

 * \file data_transfer_control.h
 * \date 2019/05/05 10:33
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief : c++��js ���ݽ������� 

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++���������ͷ�ļ�
//#include "opencv2/core/core.hpp"    
//#include "opencv2/highgui/highgui.hpp"  
//#include "opencv2/imgproc/imgproc.hpp" 
//#include "opencv2/opencv.hpp"
#include <iostream> 

class ImageProcessBase;

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();
	
	bool ParseLoadSeriesUseRapidJson(const char* json_data, std::string& js_data);
	bool ParseSwitchSeriesUseRapidJson(const char* json_data, std::string& js_data);
	bool ParseUnloadSeriesUseRapidJson(const char* json_data, std::string& js_data);	
	bool ParseImageOperationDataUseRapidJson(const char* json_data, std::string& js_data);

private:
	DataTransferController();
	static DataTransferController* instance;
	
	ImageProcessBase* image_process;
};
