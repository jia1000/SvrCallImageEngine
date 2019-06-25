
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
#include "dcmtk_dcm_loader.h"

#include "control/image_control.h"
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

struct SeriesIds
{
	std::string patient_id;
	std::string study_uid;
	std::string series_uid;	

	SeriesIds()
	{
	}
	XTOSTRUCT(O(patient_id, study_uid, series_uid
		));
};


class ImageProcessBase;
class SeriesDataInfo;


class DataTransferController
{
public:
	~DataTransferController();

	static DataTransferController* GetInstance();

	static int ParseLoadSeries(const char* json_data);
	static int ParseSwitchSeries(const char* json_data);
	static int ParseUnloadSeries(const char* json_data);	
	static int ParseImageOperationData(const char* json_data);

	static std::string GetDicomPath();
	static std::string GetMaskPath();
	static std::string GetCurvePath();

	static std::string GetPatientid();
	static std::string GetStudyuid();
	static std::string GetSeriesuid();

	static void SetSeriedIds(const struct SeriesIds& ids);
	static void SetPatientid(const std::string& id);
	static void SetStudyuid(const std::string& id);
	static void SetSeriesuid(const std::string& id);
	
	SeriesDataInfo* GerSeriresDataInfo() { return series_info; }
	DW::Control::IImageControl* GetImageControl(int control_type) { return arr_image_control[control_type]; }
private:
	DataTransferController();
	static DataTransferController* instance;
	
	static ImageProcessBase* arr_image_process[JSON_VALUE_REQUEST_TYPE_MAX];
	static DW::Control::IImageControl* arr_image_control[JSON_VALUE_REQUEST_TYPE_MAX];

	static SeriesProcessParas series_process_paras;
	static DW::IO::DcmtkDcmLoader* dcm_loader;
	static SeriesDataInfo *series_info;
};
