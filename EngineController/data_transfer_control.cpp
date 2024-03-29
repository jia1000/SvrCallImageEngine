//#include "stdafx.h"

#include "data_transfer_control.h"
#include "image_process.h"
#include "common_utils.h"
#include "series_data_info.h"
#include "render_facade.h"
#include "global_define.h"

#include "data_source.h"
#include "render_source.h"
#include "render_facade.h"
#include "tools/logger.h"
#include "io/txt_reader.h"
#include "tools/string_util.h"
#include "curve_source.h"

#include <algorithm>

#include <fstream> // ifstream, ifstream::in

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::IO;

DataTransferController* DataTransferController::instance = nullptr;

DataTransferController* DataTransferController::GetInstance()
{
	if (instance == nullptr)
		instance = new DataTransferController();
	return instance;
}

DataTransferController::DataTransferController()
 : dcm_loader(nullptr)
 , series_info(nullptr)
{
	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{		
		switch (i)
		{
		case JSON_VALUE_REQUEST_TYPE_VR:
			arr_image_process[i] = new ImageVRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_MPR:
			arr_image_process[i] = new ImageMPRProcess();
			break;
		case JSON_VALUE_REQUEST_TYPE_CPR:
			arr_image_process[i] = new ImageCPRProcess();
			break;
		default:
			arr_image_process[i] = nullptr;
			break;
		}				
	}	

	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{
		arr_image_control[i] = nullptr;
	}
 }

DataTransferController::~DataTransferController()
{
	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{
		if (arr_image_process[i])
		{
			delete arr_image_process[i];
			arr_image_process[i] = nullptr;
		}		
	}	

	for (size_t i = 0; i < JSON_VALUE_REQUEST_TYPE_MAX; i++)
	{
		arr_image_control[i] = nullptr;
	}

	if(dcm_loader)
	{
		delete dcm_loader;
		dcm_loader = nullptr;
	}

	//卸载序列时，释放资源
	if (series_info)
	{
		delete series_info;
		series_info = nullptr;
	}
}

int DataTransferController::ParseLoadSeries(const char* json_data)
{
	// initialize logger
	static bool glog_loaded = false;
	if(false == glog_loaded)
	{
		CGLogger::InitGLog("", "/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/");
		glog_loaded = true;
	}
		
	try
	{
		x2struct::X::loadjson(json_data, series_process_paras, false, true);
	}catch(...)
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	
	printf("load series dicom path : %s\n", GetDicomPath().c_str());
	
	//卸载序列时，释放资源
	if (series_info)
	{
		delete series_info;
		series_info = nullptr;
	}	
	
	series_info = new SeriesDataInfo(GetDicomPath());

    if (!series_info)
    {
		printf("fail to read dicom file.\n");
        return RET_STATUS_FAILURE;
    }
	series_info->ReadDicomFilesFromDir(true);

	// 1.1 read dcm image from directory	
	if (!dcm_loader) {
		dcm_loader = new DcmtkDcmLoader();
		printf("Dcm Loader....\n");
	}

	dcm_loader->LoadDirectory(GetDicomPath().c_str());	// only once
	
	VolData* vol_data = dcm_loader->GetData();
	if (vol_data == NULL) 
	{
		printf("fail to get volume data .\n");
		return RET_STATUS_FAILURE;
	}
	ImageDataSource::Get()->AddVolData(DataTransferController::GetInstance()->GetSeriesuid(), vol_data);	
	
	// 1.2 load mask file
	std::string mask_file_path = GetMaskPath();
	if (!mask_file_path.empty())
	{
		mask_file_path += "/maks_data.nii";
		dcm_loader->LoadVolumeMask(mask_file_path.c_str());
	}
	
	// 1.3 load curve file
	// move mpr to specified locations
	std::string curve_file_path = GetCurvePath();
	if (!curve_file_path.empty())
	{
		curve_file_path += "/vesselpoint.txt";
		vector<string> curve_data = ReadTxt(curve_file_path.c_str());
		vector<Point3f> points;
		auto it = curve_data.begin();
		while (it != curve_data.end()){

			vector<string> arr_data = Split(*it, " ");
			if (arr_data.size() >= 3){
				Point3f pnt;
				pnt.x = atof(arr_data[0].c_str());
				pnt.y = 511-atof(arr_data[1].c_str());
				pnt.z = atof(arr_data[2].c_str());

				points.push_back(pnt);
			}
			++it;
		}	
		// 1.4 create curve
		double spacings[3];
		vol_data->GetPixelData()->GetSpacing(spacings);
		// found exception
		curve_id = CurveSource::Get()->CreateCurve(GetSeriesuid(), "Vessel name", points, spacings);
		CurveSource::Get()->GetCurveById(GetSeriesuid(), curve_id);
	}

	// 2.create image control  
	static bool is_create_wnd = false;
	if (is_create_wnd == false)
	{
		// only once
		arr_image_control[JSON_VALUE_REQUEST_TYPE_VR] = RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_VR, RenderControlType::VR);
		arr_image_control[JSON_VALUE_REQUEST_TYPE_MPR] = RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_MPR, RenderControlType::MPR);
		arr_image_control[JSON_VALUE_REQUEST_TYPE_CPR] = RenderSource::Get()->CreateTwodImageControl(IMAGE_WINDOW_NAME_CPR, RenderControlType::STRETECHED_CPR);
		is_create_wnd = true;
	}
	
	RenderFacade::Get()->SetCPRCurveID(IMAGE_WINDOW_NAME_CPR, curve_id);

	return RET_STATUS_SUCCESS;
}

int DataTransferController::ParseSwitchSeries(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。		
	SeriesIds ids;
	try
	{
		x2struct::X::loadjson(json_data, ids, false, true);
	}catch(...)
	{
		printf("fail to parse switchserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	 
	printf("switch series dicom path : %s\n", GetDicomPath().c_str());
	SetSeriedIds(ids);
	
	// 3. change series 
	// 在后端调用的“切换序列”命令里，已经执行了切换操作。此处可以去掉
	RenderFacade::Get()->ChangeSeries(GetSeriesuid());
		
	return RET_STATUS_SUCCESS;
}

int DataTransferController::ParseUnloadSeries(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	SeriesIds ids;
	try
	{
		x2struct::X::loadjson(json_data, ids, false, true);
	}catch(...)
	{
		printf("fail to parse unloadserires's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}	
	SetSeriedIds(ids);
	
	printf("unload series dicom path : %s\n", GetDicomPath().c_str());

	RenderFacade::Get()->UnloadSeries(GetSeriesuid());

	return RET_STATUS_SUCCESS;
}

int DataTransferController::ParseImageOperationData(const char* json_data)
{
	// 解析从浏览器发送过来的Json数据  //json字段解析要做保护判断。
	Json::Value root;
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse imageoperation's json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	// 获得关键性的参数
	int request_type = 0;
	int ret = GetJsonDataInt(root, JSON_KEY_IMAGE_TYPE, request_type);
	
	if (ret <= 0)
	{
		return ret;
	}		

	if (request_type >= 0 && request_type < JSON_VALUE_REQUEST_TYPE_MAX)
	{
		if (arr_image_process[request_type])
		{
			arr_image_process[request_type]->Excute(json_data);
		}		
	}	

	return RET_STATUS_SUCCESS;
}

std::string DataTransferController::GetCurveId()
{
	return curve_id;
}

std::string DataTransferController::GetDicomPath()
{
	return series_process_paras.dicom_path;
}

std::string DataTransferController::GetMaskPath()
{
	return series_process_paras.mask_path;
}

std::string DataTransferController::GetCurvePath()
{
	return series_process_paras.curve_path;
}

std::string DataTransferController::GetPatientid()
{
	return series_process_paras.patient_id;
}

std::string DataTransferController::GetStudyuid()
{
	return series_process_paras.study_uid;
}

std::string DataTransferController::GetSeriesuid()
{
	return series_process_paras.series_uid;
}

void DataTransferController::SetCurveId(const std::string& id)
{
	curve_id = id;
}

void DataTransferController::SetSeriedIds(const struct SeriesIds& ids)
{
	SetPatientid(ids.patient_id);
	SetStudyuid(ids.study_uid);
	SetSeriesuid(ids.series_uid);
}

void DataTransferController::SetPatientid(const std::string& id)
{
	series_process_paras.patient_id = id;
}

void DataTransferController::SetStudyuid(const std::string& id)
{
	series_process_paras.study_uid = id;
}


void DataTransferController::SetSeriesuid(const std::string& id)
{
	series_process_paras.series_uid = id;
}

SeriesDataInfo* DataTransferController::GerSeriresDataInfo() 
{ 
	return series_info; 
}

DW::Control::IImageControl* DataTransferController::GetImageControl(int control_type) 
{ 
	if (control_type >= 0 && control_type < JSON_VALUE_REQUEST_TYPE_MAX)
	{
		return arr_image_control[control_type]; 
	}
	return nullptr;
}