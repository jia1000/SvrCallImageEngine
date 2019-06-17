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
const int SPACING_LENGHT = 3;

struct DicomInfo
{
	GIL::DICOM::DicomDataset base;
	GIL::DICOM::TagPrivadoUndefined tagBinary;
	//double position[POSITION_LENGHT];
};
struct DicomParas
{
	double position[POSITION_LENGHT];
	double spacing[SPACING_LENGHT];
	int window_width;
	int window_level;
	int bits_per_pixel;
	int bits_stored;
	int height;
	int width;
	int slice_count;
	int bits_allocated;
	double slice_location;

	std::string study_id;
	std::string series_iuid; 
	// std::string orientation;
	// std::string pixel_spacing;
	// std::string image_pixel_spacing;
};

class SeriesDataInfo
{
public:
	SeriesDataInfo(const std::string path, bool is_folder);
	~SeriesDataInfo();
	
	void GetTag(const std::string& tag, int& i);
	void GetTag(const std::string& tag, double& d);
	void GetTag(const std::string& tag, std::string& s);

	int GetSeriesDicomFileCount();
	int GetDicomDataSet(GIL::DICOM::DicomDataset& base, const int slice_index);
	int GetDicomDicomParas(DicomParas& paras, 
		const int slice_index = 0);

	unsigned char* GetPixelDataBuffer();
	unsigned int GetPixelDataLength();
	static bool SaveDicomFile(const std::string src_path_file, const std::string dst_path_file);
	
private:
	int ReadFolder(const std::string& path);
	int ReadFile(const std::string& path);
	bool GetSpacing(const int indice, 
			double& x, double& y, double& z, 
			bool isGetZAsThinkness = false);

	const std::string m_src_path;

	std::map<std::string, DicomInfo> m_bases;
	unsigned char* m_pixel_data_buffer;
	int m_pixel_data_length;	
};