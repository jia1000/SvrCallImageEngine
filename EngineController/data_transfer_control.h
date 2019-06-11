
/****************************************************************************************************

 * \file data_transfer_control.h
 * \date 2019/05/05 10:33
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief : c++和js 数据交互管理 

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

#include <iostream> 


struct SeriesProcessParas
{
	std::string dicom_path;
	std::string mask_path;
	std::string curve_path;
	std::string patient_id;
	std::string study_uid;
	std::string series_uid;
	SeriesProcessParas()
	{

	}
};

class ImageProcessBase;

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	int ParseLoadSeries(const char* json_data, std::string& js_data);
	int ParseSwitchSeries(const char* json_data, std::string& js_data);
	int ParseUnloadSeries(const char* json_data, std::string& js_data);	
	int ParseImageOperationData(const char* json_data, std::string& js_data);
	
private:
	DataTransferController();
	static DataTransferController* instance;
	
	ImageProcessBase* image_process;
public:
	static SeriesProcessParas series_process_paras;
};
