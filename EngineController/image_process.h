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

//C++���������ͷ�ļ�
//#include "opencv2/core/core.hpp"    
//#include "opencv2/highgui/highgui.hpp"  
//#include "opencv2/imgproc/imgproc.hpp" 
//#include "opencv2/opencv.hpp"
#include <iostream> 

//#include "io/dcm_reader.h"

#define JSON_KEY_REQUEST_TYPE				"request_type"
#define JSON_KEY_IMAGE_OPERATION			"image_operation"
#define JSON_KEY_IMAGE_PARAS				"image_paras"
#define JSON_KEY_IMAGE_DATA					"image_data"
#define JSON_KEY_IMAGE_SEQUENCE				"image_sequence"
#define JSON_KEY_IMAGE_MAX					"image_max"



//  ��һ�� "��������"    ��ö��
#define JSON_VALUE_REQUEST_TYPE_MPR			"mpr"
#define JSON_VALUE_REQUEST_TYPE_VR			"vr"
#define JSON_VALUE_REQUEST_TYPE_MIP			"mip"
#define JSON_VALUE_REQUEST_TYPE_CPR			"cpr"
#define JSON_VALUE_REQUEST_TYPE_TIME		"time"


//  �ڶ��� "������������"��ö��
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
	void SetKey2_ImageOperation(std::string str_opertation);
	void SetKey3_ImageOperationParas(std::string str_paras);

	virtual bool Excute(std::string& out_image_data);

protected:	
	// opencv Mat��base64�Ļ�ת
	std::string base64Decode(const char* Data, int DataByte);
	std::string base64Encode(const unsigned char* Data, int DataByte);
	//std::string Mat2Base64(const cv::Mat &img, std::string imgType);
	//cv::Mat Base2Mat(std::string &base64_data);
	//bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	bool SaveDicomFile(const std::string src_path_file, const std::string dst_path_file);
	//int req_type;
	std::string m_key1_str_req_type;	// �������ͣ���MPR VR CPR��
	std::string m_key2_str_opertation;	// ��ͬͼ��������ͣ��磬 zoom rotate move��
	std::string m_key3_str_paras;		// ��ͬͼ��������͵Ĳ���������������в�ͬ��������Ҫ����Ʒ���
	/// ��������
	std::string m_wnd_name;
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
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
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
	//GNC::GCS::Ptr<GNC::GCS::IStreamingLoader>         Loader;	
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
	std::string curve_id_;
};
