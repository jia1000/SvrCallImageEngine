//#include "stdafx.h"

#include "image_process.h"
#include "common_utils.h"
#include "global_define.h"
#include "series_data_info.h"
#include "data_transfer_control.h"

#include "api/studycontextmy.h"
#include "api/studycontextmy.h"
#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"

#include "dcmtk_dcm_loader.h"

#include "data_source.h"
#include "render_source.h"
#include "render_facade.h"
#include "io/nii_loader.h"

#include <algorithm>
#include <fstream> // ifstream, ifstream::in

#include <sstream>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diargimg.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcdict.h"

// only once
static DW::IO::IDicomReader* reader = NULL;
// static DcmtkDcmLoader* reader = nullptr;
static bool is_create_mpr_render = false;
static bool is_create_vr_render  = false;
static bool is_create_cpr_render = false;

const std::string series_name_mpr("series1");
const std::string series_name_vr("series1");
const std::string series_name_cpr("series1");

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::IO;

ImageProcessBase::ImageProcessBase()
{
}

ImageProcessBase::~ImageProcessBase()
{	
}

//////////////////////////////////////////////////////////////////////////
ImageMPRProcess::ImageMPRProcess()
	: ImageProcessBase()
{
	m_wnd_name = "mpr1";
}

ImageMPRProcess::~ImageMPRProcess()
{
}

int ImageMPRProcess::ParseJsonData(const char* json_data)
{	
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;	
}

int ImageMPRProcess::Excute(const char* json_data)
{
	printf("Operation : %s\n", m_wnd_name.c_str());
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret mpr excute : %d\n", ret);
		return ret;
	}

	// 创建文件夹
	std::string dst_dir_path(params.output_path);
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);

	// 1.read dcm image from directory
	if (!reader) {
		printf("Dcm Loader....\n");
		reader = new DW::IO::DcmtkDcmLoader();
		reader->LoadDirectory(DataTransferController::series_process_paras.dicom_path.c_str());	// only once
		
		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_mpr, vol_data);
	}

	if (!is_create_mpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateTwodImageControl(m_wnd_name, RenderControlType::MPR);	// only once
		RenderFacade::Get()->ChangeSeries(series_name_mpr);		

		is_create_mpr_render = true;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
ImageVRProcess::ImageVRProcess()
	: ImageProcessBase()
{
	m_wnd_name = "vr";
}

ImageVRProcess::~ImageVRProcess()
{
}

int ImageVRProcess::ParseJsonData(const char* json_data)
{	
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;
}

int ImageVRProcess::Excute(const char* json_data)
{
	printf("Operation : %s\n", m_wnd_name.c_str());
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret vr excute : %d\n", ret);
		return ret;
	}

	// 创建文件夹
	std::string dst_dir_path(params.output_path);
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);

	// 1.read dcm image from directory	
	if (!reader) {
		reader = new DcmtkDcmLoader();
		printf("Dcm Loader....\n");
		reader->LoadDirectory(DataTransferController::series_process_paras.dicom_path.c_str());	// only once

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_vr, vol_data);
	}
	
	if (!is_create_vr_render) {
		// 2.create image control
		RenderSource::Get()->CreateTwodImageControl(m_wnd_name, RenderControlType::VR);	// only once
		RenderFacade::Get()->ChangeSeries(series_name_vr);
		is_create_vr_render = true;
	}	

	printf("params.output_path : %s------------------------\n", params.output_path.c_str());
	RenderFacade::Get()->CreateVRRotationBatch(m_wnd_name, 
		params.output_path,// + "vr_lunei_left_right/",
		BlendMode::Composite,
		OrientationType::CORONAL,
		RotationDirection::LEFT_TO_RIGHT,
		30.0f, 1.0f, 12, -1, -1
		);

	return true;
}

//////////////////////////////////////////////////////////////////////////
ImageCPRProcess::ImageCPRProcess()
	: ImageProcessBase()
{
	m_wnd_name = "cpr";
}

ImageCPRProcess::~ImageCPRProcess()
{
}

int ImageCPRProcess::ParseJsonData(const char* json_data)
{
	try
	{
		x2struct::X::loadjson(json_data, params, false, true);
	}catch(...)
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}
	
	return RET_STATUS_SUCCESS;	
}


int ImageCPRProcess::Excute(const char* json_data)
{	
	printf("Operation : %s\n", m_wnd_name.c_str());
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret cpr excute : %d\n", ret);
		return ret;
	}
	
	// 创建文件夹
	std::string dst_dir_path(params.output_path);
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);
	
	// 1.read dcm image from directory
	if (!reader) {
		printf("Dcm Loader....\n");
		reader = new NiiImageLoader();
		std::vector<const char*> files;
		files.push_back(DataTransferController::series_process_paras.curve_path.c_str());
		reader->LoadFiles(files);	// only once
		reader->LoadVolumeMask(DataTransferController::series_process_paras.mask_path.c_str());

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		vol_data->SetDefaultWindowWidthLevel(820, 250);
		ImageDataSource::Get()->AddVolData(series_name_cpr, vol_data);
	}

	if (!is_create_cpr_render) {
		// 2.create all image control
		// RenderSource::Get()->CreateTwodImageControl(m_wnd_name, RenderControlType::STRETECHED_CPR);	// only once
		// // move mpr to specified locations
		// vector<string> curve_data = ReadTxt(DataTransferController::series_process_paras.curve_path.c_str());
		// vector<Point3f> points;
		// auto it = curve_data.begin();
		// while (it != curve_data.end()){
		// 	vector<string> arr_data = Split(*it, ",");
		// 	if (arr_data.size() >= 3){
		// 		Point3f pnt;
		// 		pnt.x = atoi(arr_data[0].c_str());
		// 		pnt.y = atoi(arr_data[1].c_str());
		// 		pnt.z = atoi(arr_data[2].c_str()) - 1;

		// 		points.push_back(pnt);
		// 	}
		// 	++it;
		// }
		// curve_id_ = CurveSource::Get()->CreateCurve(series_name_cpr, points);

		// Vector3f vx, vy;
		// float ix, iy, iz;
		
		// RenderFacade::Get()->ChangeSeries(series_name_cpr);
		// RenderFacade::Get()->SetCPRCurveID(m_wnd_name, curve_id_);
		// RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_cpr_render = true;
	}

	return true;
}