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
#include "DicomEngine/io/secondary_capture_image_generator.h"
#include "img_source.h"

// #include "dcmtk_dcm_loader.h"

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

//////////////////////////////////////////////////////////////////////////
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
	m_wnd_name = IMAGE_WINDOW_NAME_MPR;
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
		reader->LoadDirectory(DataTransferController::GetInstance()->GetDicomPath().c_str());	// only once
		
		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(DataTransferController::GetInstance()->GetSeriesuid(), vol_data);
	}

	if (!is_create_mpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateTwodImageControl(m_wnd_name, RenderControlType::MPR);	// only once
		// 在后端调用的“切换序列”命令里，已经执行了切换操作。此处可以去掉
		// RenderFacade::Get()->ChangeSeries(DataTransferController::GetInstance()->GetSeriesuid());		

		is_create_mpr_render = true;
	}

	printf("params.output_path : %s------------------------\n", params.output_path.c_str());	
	RenderFacade::Get()->CreateMPRSlabBatch (m_wnd_name, 
		params.output_path + "mpr/",
		BlendMode::MaximumIntensity,
		OrientationType::AXIAL,
		0.35f, 25.0f, 5.0f,	2000, 400
		);

	return true;
}
//////////////////////////////////////////////////////////////////////////
ImageVRProcess::ImageVRProcess()
	: ImageProcessBase()
	, control_vr(nullptr)
{
	m_wnd_name = IMAGE_WINDOW_NAME_VR;
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
	
	
	printf("params.output_path : %s------------------------\n", params.output_path.c_str());
	// 4. reconstruction VR
	RenderFacade::Get()->CreateVRRotationBatch(m_wnd_name, 
		params.output_path,
		(BlendMode)params.blend_mode,
		OrientationType::CORONAL,
		(RotationDirection)params.rotation_direction,		
		params.rotation_angle,
		params.clip_percent,
		params.output_image_number,
		params.window_width,
		params.window_level
		);

	DoTestSC();
	return true;
}

void ImageVRProcess::DoTestSC()//std::string output_path)
{
	control_vr = DataTransferController::GetInstance()->GetImageControl(JSON_VALUE_REQUEST_TYPE_VR);
	if (!control_vr)
	{
		return;
	}
	
	for(int i = 0; i < params.output_image_number; ++i)
	{
		IBitmap *bmp = control_vr->GetOutput(i);
		if(!bmp)
		{
			printf("get bitmap %d error\n", i);
			continue;
		}
		IBitmapDcmInfo *bmpInfo  =  control_vr->GetOutputInfo(i);
		if(!bmpInfo)
		{
			printf("get bitmap dcm info %d error\n", i);
			continue;
		}
		
		GIL::DICOM::DicomDataset *dataset = new GIL::DICOM::DicomDataset();
		SeriesDataInfo* series_info = DataTransferController::GetInstance()->GerSeriresDataInfo();
		if (!series_info)
		{
			return ;
		}
		
		series_info->GetDicomDataSet(*dataset, 0);

		auto *generator = new GIL::DICOM::SecondaryCaptureImageDcmGenerator(dataset);	
	
		generator->SetTag(DCM_PatientID, "test_patient_id");
		generator->SetTag(DCM_InstanceNumber, bmpInfo->GetInstanceNumber());
		double spacings[2];
		double origins[3];
		double row_v[3], col_v[3];
		bmpInfo->GetOrientation(row_v, col_v);
		bmpInfo->GetOrigin(origins);
		bmpInfo->GetPixelSpacings(spacings);
		string str_spacing = to_string(spacings[0]) + "\\" + to_string(spacings[1]);
		string str_ori = to_string(row_v[0]) + "\\" + to_string(row_v[1]) + "\\" + to_string(row_v[2])
			+ "\\" + to_string(col_v[0]) + "\\" + to_string(col_v[0]) + "\\" + to_string(col_v[2]);
		string str_origin = to_string(origins[0]) + "\\" + to_string(origins[1]) + "\\" + to_string(origins[2]);

		int instance_number = bmpInfo->GetInstanceNumber();
		generator->SetTag(DCM_ImageOrientationPatient, str_ori);
		generator->SetTag(DCM_ImagePositionPatient, str_origin);
		generator->SetTag(DCM_PixelSpacing, str_spacing);
		generator->SetTag(DCM_SliceThickness, bmpInfo->GetThickness());
		int slice_count = series_info->GetSeriesDicomFileCount();
		generator->SetTag(DCM_SeriesInstanceUID, "1.0.0.0.1.2.3.3.1." + to_string(slice_count));
		generator->SetTag(DCM_SOPInstanceUID, "1.0.0.0.1.2.3.3.1." + to_string(slice_count) + "." + to_string(instance_number));
		generator->SetTag(DCM_InstanceNumber, to_string(instance_number));
		// Series Description
		char deg[3] = {0};
		deg[0] = 0xa1;
		deg[1] = 0xe3;
		char str[30] = {0};
		sprintf(str,"%s", deg);
		generator->SetTag(DCM_SeriesDescription, "Angle: " + to_string((int)bmpInfo->GetStepValue() * (instance_number - 1)));
		
		PixelDataSource *source = new PixelDataSource(bmp);
		generator->SetPixelData(source);

		std::string dst_file_path = GeneraterDicomFileName(i);
		generator->Write(dst_file_path);
	}
}

std::string ImageVRProcess::GeneraterDicomFileName(const int iamge_index)
{
	//
	std::string dst_file_path = params.output_path;
	int angle = (int)params.rotation_angle;
	
	//完整0/颅内1
	std::stringstream ss_rule;
	ss_rule << params.generate_rule;
	std::string str_rule = ss_rule.str();
	// 绘制方式：VR 0 ， MIP 1
	std::stringstream ss_blend;
	ss_blend << params.blend_mode;
	std::string str_blend = ss_blend.str();
	// 旋转方向： LEFT_TO_RIGHT 0,  HEAT_TO_FEET 1
	std::stringstream ss_direction;
	ss_direction << params.rotation_direction;
	std::string str_direction = ss_direction.str();
	// 旋转角度
	std::stringstream ss_angle;
	ss_angle << angle * iamge_index;
	std::string str_angle = ss_angle.str();

	dst_file_path += str_rule;
	dst_file_path += "_";
	dst_file_path += str_blend;
	dst_file_path += "_";
	dst_file_path += str_direction;
	dst_file_path += "_";
	dst_file_path += str_angle;
	dst_file_path += ".dcm";

	return dst_file_path;
}

//////////////////////////////////////////////////////////////////////////
ImageCPRProcess::ImageCPRProcess()
	: ImageProcessBase()
{
	m_wnd_name = IMAGE_WINDOW_NAME_CPR;
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
		files.push_back(DataTransferController::GetInstance()->GetCurvePath().c_str());
		reader->LoadFiles(files);	// only once
		reader->LoadVolumeMask(DataTransferController::GetInstance()->GetMaskPath().c_str());

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		vol_data->SetDefaultWindowWidthLevel(820, 250);
		ImageDataSource::Get()->AddVolData(DataTransferController::GetInstance()->GetSeriesuid(), vol_data);
	}

	if (!is_create_cpr_render) {
		// 2.create all image control
		// RenderSource::Get()->CreateTwodImageControl(m_wnd_name, RenderControlType::STRETECHED_CPR);	// only once
		// // move mpr to specified locations
		// vector<string> curve_data = ReadTxt(DataTransferController::GetInstance()->GetCurvePath().c_str());
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
		
		// 在后端调用的“切换序列”命令里，已经执行了切换操作。此处可以去掉
		// // RenderFacade::Get()->ChangeSeries(DataTransferController::GetInstance()->GetSeriesuid);
		// RenderFacade::Get()->SetCPRCurveID(m_wnd_name, curve_id_);
		// RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_cpr_render = true;
	}

	return true;
}