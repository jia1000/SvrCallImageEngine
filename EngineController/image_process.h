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

#include "thirdparty/x2struct/x2struct.hpp"// json2struct header file

// class DW::Control::IImageControl;
#include "control/image_control.h"

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
	virtual int Excute(const char* json_data);

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

		XTOSTRUCT(O(image_type, blend_mode, init_orientatioin, 
		clip_percent, thickness, spacing_between_slices, 
		output_path, window_width, window_level
		));
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
	virtual int Excute(const char* json_data);

private:
	virtual int ParseJsonData(const char* json_data);
	void DoTestSC();//std::string output_path);
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

		XTOSTRUCT(O(image_type, blend_mode, init_orientatioin, 
		clip_percent, generate_rule,rotation_direction,
		rotation_angle, output_image_number, output_path,
		window_width, window_level
		));
	};
	stVRImageParams params;
	DW::Control::IImageControl *control_vr;
};

//////////////////////////////////////////////////////////////////////////
// CPR 请求类型的处理
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess();
	~ImageCPRProcess();
	virtual int Excute(const char* json_data); 

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

		XTOSTRUCT(O(image_type, vessel_name, init_orientatioin, 
		rotation_direction, rotation_angle, output_image_number, 
		output_path, window_width, window_level
		));
	};
	stCPRImageParams params;
};
