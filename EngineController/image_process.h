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

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras);
	virtual ~ImageProcessBase();

	virtual int Excute(std::string& out_image_data);
	void SetDocument(const char* json_data);

protected:
	bool SaveDicomFile(const std::string src_path_file, const std::string dst_path_file);
	
	virtual int ParseJsonData();

	void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c);
	void TryCreateDir(const std::string& dir);
	
	//int req_type;
	std::string m_key1_str_req_type;	// 请求类型，如MPR VR CPR等
	std::string m_key2_str_opertation;	// 不同图像操作类型，如， zoom rotate move等
	std::string m_key3_str_paras;		// 不同图像操作类型的参数，参数含义会有不同。具体需要见产品设计
	/// 窗口名称
	std::string m_wnd_name;
	Json::Value root;
};

//////////////////////////////////////////////////////////////////////////
// MPR 请求类型的处理
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras);
	~ImageMPRProcess();
	virtual int Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData();	
	
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
	ImageVRProcess(std::string str_paras);
	~ImageVRProcess();
	virtual int Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData();
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
	ImageCPRProcess(std::string str_paras);
	~ImageCPRProcess();
	virtual int Excute(std::string& out_image_data); // 图像缩放后数据，base64编码

private:
	virtual int ParseJsonData();
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
