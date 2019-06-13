/*=========================================================================

  Program:   ImagingEngine
  Module:    nii_loader.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "io/nii_loader.h"
#include "data/vol_data.h"
#include <vtkStringArray.h>
#include <vtkStdString.h>
#include <vtkNIFTIImageReader.h>
#include "data/pixel_data.h"
#include "tools/vtk_image_data_creator.h"

using namespace DW::IMAGE;
using namespace DW::IO;

NiiImageLoader::NiiImageLoader()
{
	volume_data_ = new VolData();
}

NiiImageLoader::~NiiImageLoader()
{
	Close();
}

bool NiiImageLoader::LoadDirectory(const char* dir)
{
	return false;
}


bool NiiImageLoader::LoadFiles(vector<const char*> files)
{
	if (files.size() == 0) return false;
	
	// 0. Create a vtkStringArray object with file paths
	const char *file_path = files[0];

	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file_path);
	niiReader->Update();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	//vtk_image_data_ = image;
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

	/// 暂时不以PixelData形式存储VolumeData
	//short *buffer_data = new short[nvox];
	//short * image_buf = static_cast<short *>(image->GetScalarPointer());
	//for (int i = 0; i < nvox; ++i)
	//	buffer_data[i] = image_buf[i];

	//////////////////////////////////////////////////////////////////////////
	/// render volume error
	// 1.1 Convert pixel data to vtkImageData using vtkImageImport
	//vtkSmartPointer<vtkImageImport> image_import = vtkSmartPointer<vtkImageImport>::New();
	//image_import->SetDataSpacing(spacings);
	//image_import->SetDataOrigin(0, 0, 0);
	//image_import->SetWholeExtent(0, dims[0], 0, dims[1], 0, dims[2]);
	//image_import->SetDataExtentToWholeExtent();
	//image_import->SetDataScalarTypeToShort();
	//image_import->SetNumberOfScalarComponents(1);
	//image_import->SetImportVoidPointer(buffer_data);
	//image_import->Update();
	//// 2. Set it to vtkImageData object
	////将图像数据缓存到vtk_image_data_指针里
	//image = image_import->GetOutput();
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	///// User VtkImageDataCreator ok
	//double origins[3] = {0.0, 0.0, 0.0};
	//VtkImageDataCreator imageDataCreator;
	//imageDataCreator.SetOrigin(origins);
	//imageDataCreator.SetSpacing(spacings);
	//imageDataCreator.SetDimensions(dims);
	//imageDataCreator.SetNumberOfComponents(1);
	//vtk_image_data_ = imageDataCreator.Create(buffer_data);
	////////////////////////////////////////////////////////////////////////////


	// 3. Convert to VolData object
	volume_data_ = new VolData();
	volume_data_->SetBitsPerPixel(16);
	volume_data_->SetBitsStored(16);
	volume_data_->SetPixelData(new VtkPixelData(image));
	volume_data_->GetPixelData()->SetBitsPerPixel(16);
	volume_data_->GetPixelData()->SetDimensions(nx, ny, nz);
	volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
	volume_data_->GetPixelData()->SetSpacing(spacings[0], spacings[1], spacings[2]);
	volume_data_->SetSliceCount(nz);
	volume_data_->SetSliceFOVWidth(460.00f);
	volume_data_->SetSliceFOVHeight(460.00f);
	volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
	volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
	volume_data_->SetBoundingBox(0,0,0,nx-1,ny-1,nz);
	volume_data_->SetDefaultWindowWidthLevel(820, 250);
	return true;
}

bool NiiImageLoader::LoadDicomData(const char* path)
{
	// 0. Create a vtkStringArray object with file paths
	const char *file_path = path;

	vtkSmartPointer<vtkNIFTIImageReader> niiReader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	niiReader->SetFileName(file_path);
	niiReader->Update();

	// 1.0 Read image data from nii file
	vtkSmartPointer<vtkImageData> image = niiReader->GetOutput();
	//vtk_image_data_ = image;
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
	
	// 3. Convert to VolData object
	volume_data_->SetBitsPerPixel(16);
	volume_data_->SetBitsStored(16);
	volume_data_->SetPixelData(new VtkPixelData(image));
	volume_data_->GetPixelData()->SetBitsPerPixel(16);
	volume_data_->GetPixelData()->SetDimensions(nx, ny, nz);
	volume_data_->GetPixelData()->SetOrigin(0, 0, 0);
	volume_data_->GetPixelData()->SetSpacing(spacings[0], spacings[1], spacings[2]);
	volume_data_->SetSliceCount(nz);
	volume_data_->SetSliceFOVWidth(460.00f);
	volume_data_->SetSliceFOVHeight(460.00f);
	volume_data_->SetStudyInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.400");
	volume_data_->SetSeriesInstanceUID("1.2.840.113619.2.278.3.679457.166.1526860577.404.3");
	volume_data_->SetBoundingBox(0,0,0,nx-1,ny-1,nz);
	volume_data_->SetDefaultWindowWidthLevel(413, 82);
	return true;
}

bool NiiImageLoader::LoadVolumeMask(const char* file)
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

	short * image_buf = static_cast<short *>(image->GetScalarPointer());
	long count = 0;
	long count2 = 0;
	int i;
	for (i = 0; i < nvox; ++i){
		mask_data[i] = image_buf[i];
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

	return true;
}

VolData* NiiImageLoader::GetData()
{
	return volume_data_;
}

void NiiImageLoader::Close()
{

}