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



#include <vector>

class DcmtkDcmLoader // public IDicomReader
{
public:
	DcmtkDcmLoader();
	~DcmtkDcmLoader();

	bool LoadDirectory(const char* dir) ;//override;
	bool LoadFiles(std::vector<const char*> files) ;//override;
	bool LoadVolumeMask(const char* file) ;//override { return false; }
	//VolData* GetData() ;//override;
	void Close() ;//override;
private:
	
};