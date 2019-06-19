
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
#include "global_define.h"

#include "thirdparty/x2struct/x2struct.hpp"// json2struct header file

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
	XTOSTRUCT(O(dicom_path, mask_path, curve_path, 
		patient_id, study_uid, series_uid
		));
};

class ImageProcessBase;
class SeriesDataInfo;

class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	int ParseLoadSeries(const char* json_data);
	int ParseSwitchSeries(const char* json_data);
	int ParseUnloadSeries(const char* json_data);	
	int ParseImageOperationData(const char* json_data);
	
private:
	DataTransferController();
	static DataTransferController* instance;
	
	ImageProcessBase* arr_image_process[JSON_VALUE_REQUEST_TYPE_MAX];
public:
	static SeriesProcessParas series_process_paras;
	static SeriesDataInfo *series_info;
};
