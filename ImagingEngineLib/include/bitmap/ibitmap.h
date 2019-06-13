/*=========================================================================

  Program:   ImagingEngine
  Module:    ibitmap.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once


namespace DW {
	namespace Bitmap {

		///
		// Represents a bitmap object with a structure to store information to save as a dicom file
		///
		class IBitmap
		{
		public:
			/// Gets image width
			virtual unsigned int GetWidth() = 0;
			/// Gets image height
			virtual unsigned int GetHeight() = 0;
			/// Gets bytes per pixel
			virtual unsigned int GetNumberOfComponent() = 0;
			/// Save to file
			virtual void Save(const char *file_path) = 0;
			/// Sets buffer data
			virtual void SetBuffer(char *data, unsigned int width, unsigned int height, unsigned int comp) = 0;
			/// Gets buffer data
			virtual const char *GetBuffer() = 0;			

		};

	}
}