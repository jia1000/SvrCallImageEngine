/*=========================================================================

  Program:   ImagingEngine
  Module:    image_control_3d.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "render/renderer_3d.h"
#include "render/imaging_3d.h"
#include "processing/image_transform.h"
#include "data/show_buffer.h"
#include "control/image_control.h"

using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::CV;

namespace DW {
	namespace Control {

		///
		// 三维图像控件
		///
		class ThreedImageControl : public IImageControl
		{
		public:
			ThreedImageControl(string control_id);
			~ThreedImageControl();

			/// 获取控件类型
			ImageControlType GetType() override { return ImageControlType::CONTROL_THREED; }
			/// 获取控件唯一编号
			string GetControlID() override;
			/// 设置图像数据源
			void SetInput(int, void *) override;
			/// 获取图像数据源
			void *GetInput(int) override;
			/// 设置图像变换执行类
			void SetTransform(IImageTransform *) override;
			/// 应用图像变换
			void Update() override;
			/// 可获取生成图像的总数
			int GetOutputNumber() override;
			/// 获取图像句柄
			IBitmap *GetOutput() override;
			IBitmap *GetOutput(int) override { return NULL; }
			/// 获取图像信息
			IBitmapDcmInfo *GetOutputInfo() override;
			IBitmapDcmInfo *GetOutputInfo(int) override { return NULL; }
			/// 设置显示图像的尺寸
			void SetDisplaySize(int width, int height) override;
			/// 翻页
			void GoTo(int pos) override;
			/// 放缩
			void Zoom(float scale) override;
			/// 旋转
			void Roll(float angle) override;
			/// 平移
			void Pan(float dx, float dy) override;
			/// 窗宽窗位
			void WWWL(int ww, int wl) override;
			/// 放缩到指定大小
			void ZoomToSize(int width, int height) override;
			/// Move the camera according to the motion vector we spend
			/// @param motionVector[] motion Vector that determines where and how much camera moves
			void Move(float motion_vector[3]) override;
			/// 绕着指定中心轴旋转一定角度
			void Rotate3D(Vector3f axis, float angle) override;

			/// 将当前数据源图像存储为bitmap
			void SaveAsBitmap(string output_dir) override;
			/// 将当前数据源图像存储为dicom图像
			void SaveAsDicom(string output_dir) override;

		private:
			int port_;
			/// Control ID
			string control_id_;
			/// Object that is rendering image
			IThreedImaging *imaging_;
			///// Calculate data
			//ICalculator *calculator_;
			/// Handle image transforming
			IImageTransform *transform_;
			/// 累计的放缩比例
			float scale_factor_;
			/// 累计的旋转角度
			float roll_angle_;
			/// 累计的水平偏移量
			float offset_x_;
			/// 累计的垂直偏移量
			float offset_y_;
			/// 窗宽窗位
			int ww_, wl_;
			/// 显示图像的宽高
			int display_width_;
			int display_height_;

		};

	}
}