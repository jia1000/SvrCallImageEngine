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

const std::string dicom_files_dir("C:\\ztest2\\dicom_test");
const std::string nii_file_data_path("C:\\ztest2\\1203_0000.nii");
const std::string nii_file_mask_path("C:\\ztest2\\1204_blood.nii");
const std::string nii_curve_data_path("C:\\ztest2\\curve_data_nii_vessel.txt");
const std::string screen_shot_file_path("C:\\ztest2\\screen_shot_temp.bmp");

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
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataInt(root, "image_type", params.image_type);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "image_type", params.image_type);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "blend_mode", params.blend_mode);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "init_orientation", params.init_orientatioin);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "clip_percent", params.clip_percent);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "thickness", params.thickness	);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "spacing_between_slices", params.spacing_between_slices);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_OUTPUT_PATH, params.output_path);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_width", params.window_width);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_level", params.window_level);
	return ret;
}

int ImageMPRProcess::Excute(const char* json_data)
{
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret mpr excute : %d\n", ret);
		return ret;
	}
	
	printf("Dcm Loader....\n");

	printf("Operation : %s\n", m_wnd_name.c_str());

	std::string src_path_file("../10.dcm");
	std::string dst_dir_path(params.output_path);
	//dst_dir_path += m_wnd_name;

	// 创建文件夹
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);
	
	//dst_dir_path += "/";
		
	int count_generate_image_number = 10;
	for (int i = 0; i < count_generate_image_number; ++i) {
		std::string dst_file_path = dst_dir_path;

		std::stringstream ss_orientation;
		ss_orientation << params.init_orientatioin;
		std::string str_rule = ss_orientation.str();

		std::stringstream ss_index;
		ss_index << i + 1;
		std::string str_index = ss_index.str();
		

		dst_file_path += str_rule;
		dst_file_path += "_";
		dst_file_path += str_index;
		dst_file_path += ".dcm";
		//printf("path : %s\n", dst_file_path.c_str());
		SeriesDataInfo::SaveDicomFile(src_path_file, dst_file_path);
	}

#if 1
	SeriesDataInfo series_info(DataTransferController::series_process_paras.dicom_path , true);
	printf("dicom_path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());
	int len = series_info.GetPixelDataLength();
	printf("dicom lenght : %d\n", len);

	// 暂时，先从本地读取Dicom文件
	// GNC::GCS::StudyContextMy* my = new GNC::GCS::StudyContextMy();
	// const std::string path_file("C:\\ztest2\\dicom_test\\413");
	// my->ReadDicomFile(path_file);

	// 1.read dcm image from directory
	std::string::size_type sz;
	
	if (!reader) {
		reader = new DW::IO::DcmtkDcmLoader();
		reader->LoadDirectory(DataTransferController::series_process_paras.dicom_path.c_str());	// only once
		
		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_mpr, vol_data);
	}

	if (!is_create_mpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::MPR);	// only once
		RenderFacade::Get()->ChangeSeries(series_name_mpr);	
		//RenderFacade::Get()->SetOrientation(wnd_mpr1_, AXIAL);
		//float pos[3] = { 255.0f, 255.0f, 0};
		RenderFacade::Get()->SetOrientation(m_wnd_name, SAGITTAL);
		RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_mpr_render = true;
	}

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	// HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	// BITMAP  bitmap ;
	// GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	// std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	// SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());
#ifdef USE_OPEN_CV
	cv::Mat src = cv::imread(screen_shot_file_path.c_str());
	out_image_data = Mat2Base64(src, "bmp");
#endif
#endif
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
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataInt(root, "image_type", params.image_type);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "blend_mode", params.blend_mode);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "init_orientation", params.init_orientatioin);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "generate_rule", params.generate_rule);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "clip_percent", params.clip_percent);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "rotation_direction", params.rotation_direction);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "rotation_angle", params.rotation_angle);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "output_image_number", params.output_image_number);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_OUTPUT_PATH, params.output_path);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_width", params.window_width);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_level", params.window_level);
	return ret;
}

int ImageVRProcess::Excute(const char* json_data)
{
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret vr excute : %d\n", ret);
		return ret;
	}
	
	printf("Dcm Loader....\n");

	printf("Operation : %s\n", m_wnd_name.c_str());

	std::string src_path_file("../10.dcm");
	std::string dst_dir_path(params.output_path);
	//dst_dir_path += m_wnd_name;

	// 创建文件夹
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);
	
	//dst_dir_path += "/";

	int angle = (int)params.rotation_angle;
	for (int i = 0; i < params.output_image_number; ++i) {
		std::string dst_file_path = dst_dir_path;

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
		ss_angle << angle * i;
		std::string str_angle = ss_angle.str();

		dst_file_path += str_rule;
		dst_file_path += "_";
		dst_file_path += str_blend;
		dst_file_path += "_";
		dst_file_path += str_direction;
		dst_file_path += "_";
		dst_file_path += str_angle;
		dst_file_path += ".dcm";
		//printf("path : %s\n", dst_file_path.c_str());
		SeriesDataInfo::SaveDicomFile(src_path_file, dst_file_path);
	}

#if 1
	// 暂时，先从本地读取Dicom文件
	//GNC::GCS::StudyContextMy* my = new GNC::GCS::StudyContextMy();
	//const std::string path_file(""C:\\ztest2\\dicom_test\\413");
	//my->ReadDicomFile(path_file);

	SeriesDataInfo series_info(DataTransferController::series_process_paras.dicom_path , true);
	printf("dicom_path : %s\n", DataTransferController::series_process_paras.dicom_path.c_str());
	int len = series_info.GetPixelDataLength();
	printf("dicom lenght : %d\n", len);

	// 1.read dcm image from directory
	
	std::string::size_type sz;
	
	printf("test1  --77777777777777777777777777777777777777777777\n");
	if (!reader) {
		reader = new DcmtkDcmLoader();
		reader->LoadDirectory(DataTransferController::series_process_paras.dicom_path.c_str());	// only once

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		ImageDataSource::Get()->AddVolData(series_name_vr, vol_data);
	}

	printf("test2  ----77777777777777777777777777777777777777777777\n");
	if (!is_create_vr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::VR);	// only once
		// RenderFacade::Get()->ChangeSeries(series_name_vr);	

		// RenderFacade::Get()->SetOrientation(m_wnd_name, CORONAL);
		// RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_vr_render = true;
	}	
	printf("params.output_path : %s------------------------\n", params.output_path.c_str());
	// RenderFacade::Get()->CreateVRRotationBatch(m_wnd_name, 
	// 	params.output_path,// + "vr_lunei_left_right/",
	// 	BlendMode::Composite,
	// 	OrientationType::CORONAL,
	// 	RotationDirection::LEFT_TO_RIGHT,
	// 	30.0f,
	// 	1.0f,
	// 	12,
	// 	-1,
	// 	-1);

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	// HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	// BITMAP  bitmap ;
	// GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	// std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	// SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());

#endif
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
	Json::Reader reader;

	if (!reader.parse(json_data, root))
	{
		printf("fail to parse json.\n");
		return RET_STATUS_JSON_PARSE_FAIL;
	}

	int ret = RET_STATUS_FAILURE;
	ret = GetJsonDataInt(root, "image_type"         	, params.image_type		);
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_VESSEL_NAME , params.vessel_name		);	
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "init_orientation"     , params.init_orientatioin);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "rotation_direction"    , params.rotation_direction);
	if(ret <= 0) return ret;
	ret = GetJsonDataFloat(root, "rotation_angle"      , params.rotation_angle	 );
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "output_image_number"   , params.output_image_number    );
	if(ret <= 0) return ret;
	ret = GetJsonDataString(root, JSON_KEY_OUTPUT_PATH , params.output_path		      );
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_width"         , params.window_width		);
	if(ret <= 0) return ret;
	ret = GetJsonDataInt(root, "window_level"         , params.window_level		);
	return ret;
}


int ImageCPRProcess::Excute(const char* json_data)
{	
	int ret = ParseJsonData(json_data);
	if (ret <= 0 ) {
		printf("ret cpr excute : %d\n", ret);
		return ret;
	}
	
	printf("Dcm Loader....\n");

	printf("Operation : %s\n", m_wnd_name.c_str());

	std::string src_path_file("../10.dcm");
	std::string dst_dir_path(params.output_path);
	//dst_dir_path += m_wnd_name;

	// 创建文件夹
	printf("Save Image to  : %s\n", dst_dir_path.c_str());
	TryCreateDir(dst_dir_path);
	
	//dst_dir_path += "/";

	int angle = (int)params.rotation_angle;
	printf("output_image_number : %d\n", params.output_image_number);
	for (int i = 0; i < params.output_image_number; ++i) {
		std::string dst_file_path = dst_dir_path;
		std::stringstream ss;
		ss << angle * i;
		std::string str_angle = ss.str();
	
		dst_file_path += params.vessel_name;
		dst_file_path += "_";
		dst_file_path += str_angle;
		dst_file_path += ".dcm";
		//printf("path : %s\n", dst_file_path.c_str());
		SeriesDataInfo::SaveDicomFile(src_path_file, dst_file_path);
	}

#if 0
	// 1.read dcm image from directory

	std::string::size_type sz;
	
	if (!reader) {
		reader = new NiiImageLoader();
		std::vector<const char*> files;
		files.push_back(nii_file_data_path.c_str());
		reader->LoadFiles(files);	// only once
		reader->LoadVolumeMask(nii_file_mask_path.c_str());

		VolData* vol_data = reader->GetData();
		if (vol_data == NULL) return false;
		vol_data->SetDefaultWindowWidthLevel(820, 250);
		ImageDataSource::Get()->AddVolData(series_name_cpr, vol_data);
	}

	if (!is_create_cpr_render) {
		// 2.create all image control
		RenderSource::Get()->CreateRenderControl(m_wnd_name, RenderControlType::STRETECHED_CPR);	// only once
		//////////////////////////////////////////////////////////////////////////
		// move mpr to specified locations
		vector<string> curve_data = ReadTxt(nii_curve_data_path.c_str());
		vector<Point3f> points;
		auto it = curve_data.begin();
		while (it != curve_data.end()){
			vector<string> arr_data = Split(*it, ",");
			if (arr_data.size() >= 3){
				Point3f pnt;
				pnt.x = atoi(arr_data[0].c_str());
				pnt.y = atoi(arr_data[1].c_str());
				pnt.z = atoi(arr_data[2].c_str()) - 1;

				points.push_back(pnt);
			}
			++it;
		}
		curve_id_ = CurveSource::Get()->CreateCurve(series_name_cpr, points);

		Vector3f vx, vy;
		float ix, iy, iz;
		
		RenderFacade::Get()->ChangeSeries(series_name_cpr);
		RenderFacade::Get()->SetCPRCurveID(m_wnd_name, curve_id_);
		RenderFacade::Get()->RenderControl(m_wnd_name);

		is_create_cpr_render = true;
	}

	// 3.get imaging object through builder. then go render and get show buffer through imaging object
	//HBITMAP hBitmap = RenderFacade::Get()->GetImageBuffer(m_wnd_name);
	//BITMAP  bitmap ;
	//GetObject (hBitmap, sizeof (BITMAP), &bitmap);

	//std::wstring ws_screenshot_file = StringToWString(screen_shot_file_path);
	//SaveBitmapToFile(hBitmap, ws_screenshot_file.c_str());
#endif
	return true;
}