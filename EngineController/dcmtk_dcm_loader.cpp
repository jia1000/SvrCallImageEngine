
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

    DicomParas paras;
    series_info.GetDicomDicomParas(paras);

	int len = series_info.GetPixelDataLength();
	printf("dicom lenght : %d\n", len);    

    // std::string patient_name = "";
    // series_info.GetTag(GKDCM_PatientName, patient_name);
    // printf("GKDCM_PatientName : %s\n", patient_name.c_str());

    unsigned char* buffer = series_info.GetPixelDataBuffer();        

    volume_data_ = new VolData();

    volume_data_->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->SetBitsStored(paras.bits_stored);

    volume_data_->SetSliceWidth(paras.window_width);
    volume_data_->SetPixelData(new RawPixelData(buffer));

    volume_data_->GetPixelData()->SetBitsPerPixel(paras.bits_per_pixel);
    volume_data_->GetPixelData()->SetDimensions(paras.height, paras.width, paras.slice_count);

    volume_data_->GetPixelData()->SetOrigin(0,0,0);//
    volume_data_->GetPixelData()->SetSpacing();

    volume_data_->SetSliceWidth(paras.width);
    volume_data_->SetSliceHeight(paras.height);

    volume_data_->SetSliceCount(paras.slice_count);
    // volume_data_->SetSliceFOVHeight(460.0f);//
    // volume_data_->SetSliceFOVWidth(460.0f);//
    volume_data_->SetStudyInstanceUID(paras.study_id);
    volume_data_->SetSeriesInstanceUID(paras.series_iuid);
    volume_data_->SetBoundingBox(0, 0, 0, paras.height - 1, paras.width - 1, paras.slice_count - 1);
    volume_data_->SetDefaultWindowWidthLevel(paras.window_width, paras.window_level);

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