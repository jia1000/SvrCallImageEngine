/****************************************************************************************************

 * \file image_process.h
 * \date 2019/05/06 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

//C++输入输出库头文件
//#include "opencv2/core/core.hpp"    
//#include "opencv2/highgui/highgui.hpp"  
//#include "opencv2/imgproc/imgproc.hpp" 
//#include "opencv2/opencv.hpp"
#include <iostream> 
#include <vector>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

//#include "io/dcm_reader.h"

#define JSON_KEY_IMAGE_TYPE 				"image_type"



struct stCPRImageParams
{
	stCPRImageParams()
	{
		image_type			= 0;
		vessel_name			= "";
		init_orientatioin	= 0;
		rotation_direction	= 0;
		rotation_angle		= 0;
		output_image_number = 0;
		output_path			= "";
		window_width		= 0;
		window_level		= 0;	
	}
	int image_type;
	std::string vessel_name;
	int init_orientatioin;
	int rotation_direction;
	float rotation_angle;
	int output_image_number;
	std::string output_path;
	int window_width;
	int window_level;	
};

struct stVRImageParams
{
	stVRImageParams()
	{
		image_type			= 0;
		blend_mode			= 0;
		init_orientatioin	= 0;
		clip_percent		= 0;
		generate_rule		= 0;
		rotation_direction	= 0;
		rotation_angle		= 0;
		output_image_number = 0;
		output_path			= "";
		window_width		= 0;
		window_level		= 0;	
	}
	int image_type;
	int blend_mode;
	int init_orientatioin;
	float clip_percent;
	int generate_rule;
	int rotation_direction;
	float rotation_angle;
	int output_image_number;
	std::string output_path;
	int window_width;
	int window_level;	
};

struct stMPRMIPImageParams
{
	stMPRMIPImageParams()
	{
		image_type				= 0;
		blend_mode				= 0;
		init_orientatioin		= 0;
		clip_percent			= 0;
		thickness				= 0;
		spacing_between_slices	= 0;
		//output_image_number		= 0;
		output_path				= "";
		window_width			= 0;
		window_level			= 0;	
	}
	int image_type;
	int blend_mode;
	int init_orientatioin;
	float clip_percent;
	float thickness;
	float spacing_between_slices;
	//int output_image_number;
	std::string output_path;
	int window_width;
	int window_level;	
};

#define JSON_KEY_REQUEST_TYPE				"request_type"
#define JSON_KEY_IMAGE_OPERATION			"image_operation"
#define JSON_KEY_IMAGE_PARAS				"image_paras"
#define JSON_KEY_IMAGE_DATA					"image_data"
#define JSON_KEY_IMAGE_SEQUENCE				"image_sequence"
#define JSON_KEY_IMAGE_MAX					"image_max"



//  第一级 "请求类型"    的枚举
#define JSON_VALUE_REQUEST_TYPE_VR			0
#define JSON_VALUE_REQUEST_TYPE_MPR			1
#define JSON_VALUE_REQUEST_TYPE_CPR			2


#define JSON_VALUE_REQUEST_TYPE_MIP			"mip"
#define JSON_VALUE_REQUEST_TYPE_TIME		"time"


//  第二级 "操作操作类型"的枚举
#define JSON_VALUE_IMAGE_OPERATION_ZOOM		"zoom"
#define JSON_VALUE_IMAGE_OPERATION_ROTATE	"rotate"
#define JSON_VALUE_IMAGE_OPERATION_MOVE		"move"
#define JSON_VALUE_IMAGE_OPERATION_SKIP		"skip"

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras);
	virtual ~ImageProcessBase();

	//void SetKey1_RequestType(std::string str_req_type);
	//void SetKey2_ImageOperation(std::string str_opertation);
	//void SetKey3_ImageOperationParas(std::string str_paras);

	virtual bool Excute(std::string& out_image_data);
	void SetDocument(const char* json_data) { doc.Parse(json_data); }

protected:	
	// opencv Mat和base64的互转
	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	//std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	//cv::Mat Base2Mat(std::string &base64_data);
	//bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	bool SaveDicomFile(const std::string src_path_file, const std::string dst_path_file);
	
	virtual bool ParseJsonData();
	int GetJsonDataInt(std::string key);
	std::string GetJsonDataString(std::string key);
	float GetJsonDataFloat(std::string key);
	void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c);
	void TryCreateDir(const std::string& dir);
	
	//int req_type;
	std::string m_key1_str_req_type;	// 请求类型，如MPR VR CPR等
	std::string m_key2_str_opertation;	// 不同图像操作类型，如， zoom rotate move等
	std::string m_key3_str_paras;		// 不同图像操作类型的参数，参数含义会有不同。具体需要见产品设计
	/// 窗口名称
	std::string m_wnd_name;
	Document doc;
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras);
	~ImageMPRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual bool ParseJsonData();

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
	stMPRMIPImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// VR 请求类型的处理
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess(std::string str_paras);
	~ImageVRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual bool ParseJsonData();

	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
	stVRImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// CPR 请求类型的处理
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess(std::string str_paras);
	~ImageCPRProcess();
	virtual bool Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual bool ParseJsonData();
	std::string curve_id_;
	stCPRImageParams params;
};
