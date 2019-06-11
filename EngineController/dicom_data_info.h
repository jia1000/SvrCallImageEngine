/****************************************************************************************************

 * \file dicom_controller.h
 * \date 2019/06/10 14:20
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once

#include "main/controllers/dicommanager.h"
#include "api/dicom/dicomdataset.h"
#include "api/dicom/dcmdictionary.h"
#include "api/dicom/idicom.h"

#include <map>

const int POSITION_LENGHT = 3;
typedef struct DicomInfo
{
	GIL::DICOM::DicomDataset base;
	GIL::DICOM::TagPrivadoUndefined tagBinary;
	double position[POSITION_LENGHT];
};

class SeriesDataInfo
{
public:
	SeriesDataInfo(const std::string path, bool is_folder);
	~SeriesDataInfo();
	
	void GetTag(const std::string& tag, int& i);
	void GetTag(const std::string& tag, double& d);
	void GetTag(const std::string& tag, std::string& s);

	unsigned char* GetPixelDataBuffer();
	unsigned int GetPixelDataLength();
private:
	int ReadFolder();
	int ReadFile(const std::string& path);
	const std::string m_src_path;
	//GIL::DICOM::DicomDataset base;
	//GIL::DICOM::IDICOMManager*	pDICOMManager;

	std::map<std::string, DicomInfo> m_bases;
	unsigned char* m_pixel_data_buffer;
	int m_pixel_data_length;	
};