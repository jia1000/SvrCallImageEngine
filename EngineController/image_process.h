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

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

#define JSON_KEY_IMAGE_TYPE 				"image_type"

//  ��һ�� "��������"    ��ö��
#define JSON_VALUE_REQUEST_TYPE_VR			0
#define JSON_VALUE_REQUEST_TYPE_MPR			1
#define JSON_VALUE_REQUEST_TYPE_CPR			2


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

class ImageProcessBase
{
public:
	ImageProcessBase(std::string str_paras);
	virtual ~ImageProcessBase();

	virtual bool Excute(std::string& out_image_data);
	void SetDocument(const char* json_data) { doc.Parse(json_data); }

protected:
	bool SaveDicomFile(const std::string src_path_file, const std::string dst_path_file);
	
	virtual bool ParseJsonData();
	int GetJsonDataInt(std::string key);
	std::string GetJsonDataString(std::string key);
	float GetJsonDataFloat(std::string key);
	void SplitString(const std::string& src, std::vector<std::string>& v, const std::string& c);
	void TryCreateDir(const std::string& dir);
	
	//int req_type;
	std::string m_key1_str_req_type;	// �������ͣ���MPR VR CPR��
	std::string m_key2_str_opertation;	// ��ͬͼ��������ͣ��磬 zoom rotate move��
	std::string m_key3_str_paras;		// ��ͬͼ��������͵Ĳ���������������в�ͬ��������Ҫ����Ʒ���
	/// ��������
	std::string m_wnd_name;
	Document doc;
};

//////////////////////////////////////////////////////////////////////////
// MPR �������͵Ĵ���
class ImageMPRProcess : public ImageProcessBase
{
public:
	ImageMPRProcess(std::string str_paras);
	~ImageMPRProcess();
	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	virtual bool ParseJsonData();	
	stMPRMIPImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// VR �������͵Ĵ���
class ImageVRProcess : public ImageProcessBase
{
public:
	ImageVRProcess(std::string str_paras);
	~ImageVRProcess();
	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	virtual bool ParseJsonData();
	stVRImageParams params;
};

//////////////////////////////////////////////////////////////////////////
// CPR �������͵Ĵ���
class ImageCPRProcess : public ImageProcessBase
{
public:
	ImageCPRProcess(std::string str_paras);
	~ImageCPRProcess();
	virtual bool Excute(std::string& out_image_data); // ͼ�����ź����ݣ�base64����

private:
	virtual bool ParseJsonData();
	std::string curve_id_;
	stCPRImageParams params;
};
