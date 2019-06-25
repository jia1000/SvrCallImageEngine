
// #include "dcmtk_dcm_loader.h"
#include "series_data_info.h"
#include "data_transfer_control.h"
#include "series_data_info.h"

using namespace DW::IMAGE;
using namespace DW::IO;

DcmtkDcmLoader::DcmtkDcmLoader()
 : volume_data_(NULL)
{
    
}

DcmtkDcmLoader::~DcmtkDcmLoader()
{
	if(volume_data_)
    {
        delete volume_data_;
        volume_data_ = NULL;
    }
    
}

bool DcmtkDcmLoader::LoadDirectory(const char* dir) 
{
    
    
    DicomParas paras;
    memset(&paras, 0, sizeof(DicomParas));

    SeriesDataInfo* series_info = DataTransferController::GetInstance()->GerSeriresDataInfo();
    if (!series_info)
    {
        return false;
    }
    
    series_info->GetDicomDicomParas(paras); 
    
    unsigned char* buffer = series_info->GetPixelDataBuffer();        

    if(volume_data_)
    {
        delete volume_data_;
        volume_data_ = NULL;
    }

    volume_data_ = new VolData();

    volume_data_->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->SetBitsStored(paras.bits_stored);
    volume_data_->SetSliceWidth(paras.window_width);
    volume_data_->SetPixelData(new RawPixelData((char *)buffer));
    volume_data_->GetPixelData()->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->GetPixelData()->SetDimensions(paras.height, paras.width, paras.slice_count);
    volume_data_->GetPixelData()->SetOrigin(0,0,0);//
    volume_data_->GetPixelData()->SetSpacing(paras.spacing[0], paras.spacing[1], paras.spacing[2]);
    volume_data_->SetSliceWidth(paras.width);
    volume_data_->SetSliceHeight(paras.height);
    volume_data_->SetSliceCount(paras.slice_count);
    // volume_data_->SetSliceFOVHeight(460.0f);//
    // volume_data_->SetSliceFOVWidth(460.0f);//
    volume_data_->SetStudyInstanceUID(paras.study_id);
    volume_data_->SetSeriesInstanceUID(paras.series_iuid);
    volume_data_->SetBoundingBox(0, 0, 0, paras.height - 1, paras.width - 1, paras.slice_count - 1);
    volume_data_->SetDefaultWindowWidthLevel(paras.window_width, paras.window_level);
    printf("spacing : %.5f, %.5f, %.5f\n", paras.spacing[0], paras.spacing[1], paras.spacing[2]);
    printf("window_width : %d, window_level : %d\n", paras.window_width, paras.window_level);

    return true;
}

bool DcmtkDcmLoader::LoadFiles(std::vector<const char*> files)
{
    return true;
}
bool DcmtkDcmLoader::LoadDicomData(const char* dir)
{
    return true;
}
bool DcmtkDcmLoader::LoadVolumeMask(const char* file) 
{
    return true;
}
VolData* DcmtkDcmLoader::GetData() 
{
    return  volume_data_;
}

void DcmtkDcmLoader::Close() 
{
    
}