/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_generator_cpr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_generator_cpr.h"

using namespace DW::Batch;


CPRRotationBatchGenerator::CPRRotationBatchGenerator()
{

}

CPRRotationBatchGenerator::~CPRRotationBatchGenerator()
{

}

void CPRRotationBatchGenerator::Execute(BatchArgs *args)
{
	CPRRotationBatchArgs *batch_args = static_cast<CPRRotationBatchArgs *>(args);
	if (NULL != batch_args)
	{
		string output_path = batch_args->GetOutputPath();
		float step_angle = batch_args->GetStep();
		int image_number = batch_args->GetImageNumber();
		string curve_id = batch_args->GetCurveId();
		RotationDirection direction = batch_args->GetDirection();
		IThreedImaging *imaging = batch_args->GetImaging();

		IThreedRenderer *renderer = imaging->GetRenderer();
		//TODO 设定初始位置判断与旋转轴有关
		Camera *camera = renderer->GetCamera();
		if (camera){
			camera->SetCameraViewPlane(OrientationType::CORONAL);
		}
		//TODO 列填充可以水平旋转，行填充可以垂直旋转
		float angle = 0.0f;
		Vector3f axis_vec(0.0f, 0.0f, 1.0f);
		switch (direction)
		{
		case DW::IMAGE::LEFT_TO_RIGHT:
			break;
		case DW::IMAGE::RIGHT_TO_LEFT:
			axis_vec[2] = -1.0f;
			break;
		case DW::IMAGE::HEAD_TO_FEET:
			axis_vec[0] = 1.0f;
			axis_vec[1] = 0.0f;
			axis_vec[2] = 0.0f;
			break;
		case DW::IMAGE::FEET_TO_HEAD:
			axis_vec[0] = 1.0f;
			axis_vec[1] = 0.0f;
			axis_vec[2] = 0.0f;
			break;
		default:
			break;
		}
		float count_angle = 0.0f;
		int i;
		for(i=0; i<image_number; ++i){

			if (i == 0){
				imaging->Render();
			}
			else{
				imaging->Rotate3D(axis_vec, step_angle);
			}

			ShowBuffer *buffer = new ShowBuffer();
			buffer->DeepCopy(imaging->GetShowBuffer());

			ImagePlane *plane = new ImagePlane();
			plane->DeepCopy(imaging->GetCurrentImagePlane());
			
			// Notify batch image source to cache this buffer
			if (this->GetCallback()){
				string path = output_path + curve_id + "_" + to_string(count_angle) + ".bmp";
				BufferResult *result = new BufferResult();
				result->buffer_data = buffer;
				result->image_plane = plane;
				result->instance_number = i + 1;
				result->file_name = path;
				result->step = step_angle;
				batch_args->GetWWWL(result->window_width, result->window_level);
				this->GetCallback()->OnBatchOneGenerated(result, true);
			}

			// 累加角度
			count_angle += step_angle;
		}

		if (this->GetCallback()){
			this->GetCallback()->OnBatchAllGenerated(1);
		}
	}
	else{
		if (this->callback_){
			this->callback_->OnBatchAllGenerated(0);
		}
	}
}

void CPRRotationBatchGenerator::Destroy()
{

}