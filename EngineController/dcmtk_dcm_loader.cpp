
#include "series_data_info.h"
#include "data_transfer_control.h"
#include "series_data_info.h"

#include <vtkNIFTIImageReader.h>

using namespace DW::IMAGE;
using namespace DW::IO;

DcmtkDcmLoader::DcmtkDcmLoader()
 : volume_data_(NULL)
{
    
}

DcmtkDcmLoader::~DcmtkDcmLoader()
{
	    
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
    vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file);
	niiReader->Update();

	int type = niiReader->GetDataScalarType();
	int com = niiReader->GetNumberOfScalarComponents();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	
	// 2.0 Convert to unsigned char data pointer
	int dims[3];
	image->GetDimensions(dims);

	int nx, ny, nz;
	float dx, dy, dz;
	int nvox;

	nx = dims[0], ny = dims[1], nz = dims[2];
	nvox = nx * ny * nz;

	double spacings[3];
	image->GetSpacing(spacings);
	dx = spacings[0], dy = spacings[1], dz = spacings[2];
	UNITMARK3D *mask_data = new UNITMARK3D[nvox];

	char * image_buf = static_cast<char *>(image->GetScalarPointer());
	long count = 0;
	long count2 = 0;
	int x, y, z, i;
	// from head to feet
	int width = nx;
	int height = ny;
	//for(i=0; i<nz; ++i){
	//	memcpy(mask_data + i * plane_size, image_buf + (nz-1 - i) * plane_size, plane_size);
	//}

	int x_min = 10000, x_max = 0, y_min = 10000, y_max = 0, z_min = 10000, z_max = 0, val = 0;
	
	int plane_size = width * height;
	for (z=0; z<nz; ++z){
		for (y=0; y<height; ++y){
			for (x=0; x<width; ++x){
				
				// VolumeMask 不需要y轴坐标转换，因为Mask后续还会再做一次转换，与其这里就不做转换了。
				/*mask_data[z*plane_size + y*width + x] = image_buf[(nz-1 - z)*plane_size + (height-1 - y) * width + x];*/
				mask_data[z*plane_size + y*width + x] = image_buf[(nz-1 - z)*plane_size + y* width + x];

				val =  mask_data[z*plane_size + y*width + x];
				if (val > 0){
					if (x_min > x){
						x_min = x;
					}
					if (x_max < x){
						x_max = x;
					}
					if (y_min > y){
						y_min = y;
					}
					if (y_max < y){
						y_max = y;
					}
					if (z_min > z){
						z_min = z;
					}
					if (z_max < z){
						z_max = z;
					}
				}
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////
	///// User VtkImageDataCreator ok
	//double origins[3] = {0.0, 0.0, 0.0};
	//VtkImageDataCreator imageDataCreator;
	//imageDataCreator.SetOrigin(origins);
	//imageDataCreator.SetSpacing(spacings);
	//imageDataCreator.SetDimensions(dims);
	//imageDataCreator.SetNumberOfComponents(1);
	//image = imageDataCreator.Create(mask_data);
	////////////////////////////////////////////////////////////////////////////

	volume_data_->SetMark(mask_data);
	volume_data_->SetMarkBoundingBox(x_min, y_min, z_min, x_max, y_max, z_max);

	return true;
}
VolData* DcmtkDcmLoader::GetData() 
{
    return  volume_data_;
}

void DcmtkDcmLoader::Close() 
{
    
}