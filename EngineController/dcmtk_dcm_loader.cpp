
#include "dcmtk_dcm_loader.h"
#include "series_data_info.h"


using namespace DW::IMAGE;
using namespace DW::IO;

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

    int bits_per_pixel = 0;
    series_info.GetTag(GKDCM_BitsAllocated, bits_per_pixel);//?


    int bits_stored = 0;
    series_info.GetTag(GKDCM_BitsStored, bits_stored);

    int dimensions_x = 0;
    series_info.GetTag(GKDCM_Rows, dimensions_x);

    int dimensions_y = 0;
    series_info.GetTag(GKDCM_Columns, dimensions_x);


    unsigned char* buffer = series_info.GetPixelDataBuffer();    

    volume_data_ = new VolData();
    volume_data_->SetSliceWidth(window_width);
    volume_data_->SetPixelData(new RawPixelData(buffer));
}

bool DcmtkDcmLoader::LoadFiles(std::vector<const char*> files)
{

}
bool DcmtkDcmLoader::LoadDicomData(const char* dir)
{

}
bool DcmtkDcmLoader::LoadVolumeMask(const char* file) 
{
    return false;
}
VolData* DcmtkDcmLoader::GetData() 
{
    return  volume_data_;
}

void DcmtkDcmLoader::Close() 
{
    
}