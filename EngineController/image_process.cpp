//#include "stdafx.h"

#include "image_process.h"
#include "common_utils.h"
#include "global_define.h"
#include "series_data_info.h"
#include "data_transfer_control.h"
#include "ginkgouid.h"

#include "api/studycontextmy.h"
#include "api/studycontextmy.h"
#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"
#include "DicomEngine/io/secondary_capture_image_generator.h"
#include "img_source.h"

#include "data_source.h"
#include "render_source.h"
#include "render_facade.h"
#include "io/nii_loader.h"
#include "io/ct_image_generator.h"

#include <algorithm>
#include <fstream> // ifstream, ifstream::in

#include <sstream>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimage/diargimg.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcdict.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::IO;

//////////////////////////////////////////////////////////////////////////
ImageProcessBase::ImageProcessBase()
: control_vr(nullptr)
{
}

ImageProcessBase::~ImageProcessBase()
{	
}

void ImageProcessBase::DoTest(int control_type, bool is_mpr)
{
	if (control_type < 0 || control_type >= JSON_VALUE_REQUEST_TYPE_MAX)
	{
		printf("the paras 'control_type' is error.\n");
		return;
	}
	
	control_vr = DataTransferController::GetInstance()->GetImageControl(control_type);
	if (!control_vr)
	{
		return;
	}
	int output_number = control_vr->GetOutputNumber();
	for(int i = 0; i < output_number; ++i)
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

		SeriesDataInfo* series_info = DataTransferController::GetInstance()->GerSeriresDataInfo();
		if (!series_info)
		{
			delete bmp;
			delete bmpInfo;
			return ;
		}
		
		GIL::DICOM::DicomDataset dataset;
		series_info->GetDicomDataSet(dataset, 0);

		GIL::DICOM::CTImageDcmGenerator *generator = new GIL::DICOM::CTImageDcmGenerator(&dataset);
		// test git
		//generator->SetTag(DCM_PatientID, "test_patient_id");
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

		int series_counter = series_info->GetSeriesDicomFileCount();
		std::string generate_series_uid = GIL::DICOM::MakeUID(GIL::DICOM::GUID_SeriesRoot, series_counter);
		generator->SetTag(DCM_SeriesInstanceUID, generate_series_uid);
		generator->SetTag(DCM_SOPInstanceUID, generate_series_uid + "." + to_string(instance_number));
		generator->SetTag(DCM_InstanceNumber, to_string(instance_number));
		// Series Description
		if (is_mpr){
			generator->SetTag(DCM_SeriesDescription, "Position: " + to_string((int)bmpInfo->GetStepValue() * (instance_number - 1)) + "mm");
		}
		else{
			char deg[3] = {0};
			deg[0] = 0xa1;
			deg[1] = 0xe3;
			char str[30] = {0};
			sprintf(str,"%s", deg);
			generator->SetTag(DCM_SeriesDescription, "Angle: " + to_string((int)bmpInfo->GetStepValue() * (instance_number - 1)));
		}

		int ww, wl;
		bmpInfo->GetWindowLevel(ww, wl);
		generator->SetTag(DCM_WindowWidth, to_string(ww));
		generator->SetTag(DCM_WindowCenter, to_string(wl));

		PixelDataSource *source = new PixelDataSource(bmp);
		generator->SetPixelData(source);

		std::string dst_file_path = GeneraterDicomFileName(i);
		generator->Write(dst_file_path);
		
		delete generator;
		delete bmp;
		delete bmpInfo;			
	}
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

	printf("params.output_path : %s------------------------\n", params.output_path.c_str());	
	RenderFacade::Get()->CreateMPRSlabBatch (m_wnd_name, 
		params.output_path,
		(BlendMode)params.blend_mode,
		(OrientationType)params.init_orientatioin,
		params.clip_percent,
		params.thickness,
		params.spacing_between_slices,
		params.window_width,
		params.window_level
		);

	DoTest(JSON_VALUE_REQUEST_TYPE_MPR, true);
	return true;
}

std::string ImageMPRProcess::GeneraterDicomFileName(const int iamge_index)
{
	//
	std::string dst_file_path = params.output_path;
	
	std::stringstream ss_orientation;
	ss_orientation << params.init_orientatioin;
	std::string str_rule = ss_orientation.str();

	std::stringstream ss_index;
	ss_index << iamge_index + 1;
	std::string str_index = ss_index.str();	

	dst_file_path += str_rule;
	dst_file_path += "_";
	dst_file_path += str_index;
	dst_file_path += ".dcm";
	
	return dst_file_path;
}
//////////////////////////////////////////////////////////////////////////
ImageVRProcess::ImageVRProcess()
	: ImageProcessBase()
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
		(OrientationType)params.init_orientatioin,//OrientationType::CORONAL,
		(RotationDirection)params.rotation_direction,		
		params.rotation_angle,
		params.clip_percent,
		params.output_image_number,
		params.window_width,
		params.window_level
		);

	DoTest(JSON_VALUE_REQUEST_TYPE_VR, false);
	return true;
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


	RenderFacade::Get()->CreateCPRRotationBatch(m_wnd_name, 
		params.output_path,//output_dir + "cpr/",
		DataTransferController::GetInstance()->GetCurveId(),
		(OrientationType)params.init_orientatioin,// OrientationType::CORONAL,
		(RotationDirection)params.rotation_direction,// RotationDirection::LEFT_TO_RIGHT,
		params.rotation_angle,
		params.output_image_number,
		params.window_width,
		params.window_level
		);
	
	DoTest(JSON_VALUE_REQUEST_TYPE_CPR, false);

	return true;
}

std::string ImageCPRProcess::GeneraterDicomFileName(const int iamge_index)
{
	std::string dst_file_path = params.output_path;
	int angle = (int)params.rotation_angle;

	std::stringstream ss_angle;
	ss_angle << iamge_index * angle;
	std::string str_angle = ss_angle.str();


	dst_file_path += params.vessel_name;
	dst_file_path += "_";
	dst_file_path += str_angle;
	dst_file_path += ".dcm";

	return dst_file_path;
}
