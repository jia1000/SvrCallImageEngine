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

#include <iostream> 
#include <vector>

#include "common_utils.h"
#include "global_define.h"

class ImageProcessBase
{
public:
	ImageProcessBase();
	virtual ~ImageProcessBase();

	virtual int Excute(const char* json_data)
	{
		return RET_STATUS_SUCCESS;
	}
	virtual int ParseJsonData() 
	{
		return RET_STATUS_SUCCESS;
	}

protected:
	// 窗口名称
	std::string m_wnd_name;
	Json::Value root;
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess();
	~ImageMPRProcess();
	virtual int Excute(const char* json_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData(const char* json_data);	
	
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
		std::string output_path;
		int window_width;
		int window_level;	
	};
	stMPRMIPImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// VR 请求类型的处理
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess();
	~ImageVRProcess();
	virtual int Excute(const char* json_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData(const char* json_data);
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
	stVRImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// CPR 请求类型的处理
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess();
	~ImageCPRProcess();
	virtual int Excute(const char* json_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData(const char* json_data);
	std::string curve_id_;
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
	stCPRImageParams params;
};
