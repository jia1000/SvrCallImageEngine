/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_raycasting_gpu.h
  author: 	 zhangjian
  Brief:	 VRͼ���raycasting�ؽ�������Ⱦ�࣬����GPU

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "render/renderer_raycasting_cpu.h"
#include "render/renderer_vr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RayCastingRendererGPU : public VolumeRenderer
		{
		public:
			RayCastingRendererGPU();
			~RayCastingRendererGPU();
		protected:
			void DoRender(vtkSmartPointer<vtkImageData> imagedata) override;
		private:

		};
	}
}