
#include "dcmtk_dcm_loader.h"
#include "series_data_info.h"

DcmtkDcmLoader::DcmtkDcmLoader()
{
    
}

DcmtkDcmLoader::~DcmtkDcmLoader()
{
	
}

bool DcmtkDcmLoader::LoadDirectory(const char* dir) 
{
    SeriesDataInfo series_info(dir , true);

	int len = series_info.GetPixelDataLength();
	printf("dicom lenght : %d\n", len);

    int window_width = 0;
    series_info.GetTag(GKDCM_WindowWidth, window_width);
    unsigned char* buffer = series_info.GetPixelDataBuffer();    

    // volume_data_ = new VolData();
    // volume_data_->SetSliceWidth(window_width);
    // volume_data_->SetPixelData(buffer);
}

bool DcmtkDcmLoader::LoadFiles(std::vector<const char*> files)
{

}
bool DcmtkDcmLoader::LoadVolumeMask(const char* file) 
{

}
//VolData* GetData() 
//{
//}
void DcmtkDcmLoader::Close() 
{
    
}