/*=========================================================================

  Program:   ImagingEngine
  Module:    renderer_raycasting_cpu.h
  author: 	 zhangjian
  Brief:	 VR图像的raycasting重建方法渲染类，是基于cpu计算的，性能低

=========================================================================*/
#pragma once

#include "vtk_include.h"
#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/pick_3d.h"
#include "render/measure_3d.h"
#include "render/orientation_3d.h"
#include "data/pixel_data.h"
#include "render/transfer_function.h"
#include "render/renderer_vr.h"

using namespace DW::IMAGE;

namespace DW {
	namespace Render{

		class RayCastingRenderer : public VolumeRenderer
		{
		public:
			RayCastingRenderer();
			virtual ~RayCastingRenderer();
			
		protected:
			virtual void DoRender(vtkSmartPointer<vtkImageData> imagedata) override;
			void DoRender2(vtkSmartPointer<vtkImageData> imagedata);

		};
	}
}