/*=========================================================================

  Program:   ImagingEngine
  Module:    batch_vr.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "batch/batch_generator_vr.h"
#include <thread>
#include "vtkPlanes.h"
#include "render/renderer_vr.h"
#include "render/render_param_vr.h"

using namespace DW::Batch;


VRRotationBatchGenerator::VRRotationBatchGenerator()
{

}

VRRotationBatchGenerator::~VRRotationBatchGenerator()
{

}

void VRRotationBatchGenerator::Execute(BatchArgs *args)
{
	VRRotationBatchArgs *batch_args = static_cast<VRRotationBatchArgs *>(args);
	if (NULL != batch_args)
	{
		try
		{
			string output_path = batch_args->GetOutputPath();
			OrientationType ori = batch_args->GetOrientation();
			float step_angle = batch_args->GetStep();
			int image_number = batch_args->GetImageNumber();
			float clip = 1.0f - batch_args->GetClipPercent();
			RotationDirection direction = batch_args->GetDirection();
			BlendMode mode = batch_args->GetBlendMode();
			int ww, wl;
			batch_args->GetWWWL(ww, wl);

			string str_msg = " output path: " + output_path + "\r rotation: " + to_string(direction) + "\r mode: " + to_string(mode) + "\r image: " + to_string(image_number) + "\r clip: " + to_string(clip) + "\r ww/wl: " + to_string(ww) + "/" + to_string(wl);
			CGLogger::Info("VRRotationBatchGenerator::Execute with the following parameters:\r\n" + str_msg);

			IThreedImaging *imaging = batch_args->GetImaging();
			IThreedRenderer *renderer = imaging->GetRenderer();
			//TODO �趨��ʼλ���ж�
			Camera *camera = renderer->GetCamera();
			if (camera){
				camera->SetCameraViewPlane(ori);		//OrientationType::CORONAL
			}
			// Set window size
			renderer->GetRenderParam()->SetWidth(768);
			renderer->GetRenderParam()->SetHeight(768);
			 
			// Set flag to do reset camera operation in renderer
			renderer->SetFirstRenderFlag(true);
			VolData *data = imaging->GetData();
			double image_spacings[3];
			data->GetPixelData()->GetSpacing(image_spacings);
			int image_width = data->GetSliceWidth();
			int image_height = data->GetSliceHeight();
			int image_count = data->GetSliceCount();

			float angle = 0.0f;
			Vector3f axis_vec(0.0f, 1.0f, 0.0f);
			switch (direction)
			{
			case DW::IMAGE::LEFT_TO_RIGHT:
				break;
			case DW::IMAGE::RIGHT_TO_LEFT:
				axis_vec[0] = 0.0f;
				axis_vec[1] = -1.0f;
				break;
			case DW::IMAGE::HEAD_TO_FEET:
				axis_vec[0] = 1.0f;
				axis_vec[1] = 0.0f;
				break;
			case DW::IMAGE::FEET_TO_HEAD:
				axis_vec[0] = -1.0f;
				axis_vec[1] = 0.0f;
				break;
			default:
				break;
			}
			// �����VMIP��Ҫ�����ô���λ
			if (mode == BlendMode::MaximumIntensity ||
				mode == BlendMode::MinimumIntensity){
				imaging->WindowWidthLevel(ww, wl);
			}
			// ����clipping planes
			VolumeRenderer *vol_renderer = dynamic_cast<VolumeRenderer *>(renderer);
			// ����BlendMode
			vol_renderer->SetBlendMode(mode);
			if (clip > 0.0f && 1.0f - clip > MathTool::kEpsilon){
				// ƽ��
				if (camera){
					float offset[3] = { 0.0f };
					double actual_height = (double)image_count * image_spacings[2];
					if (ori == OrientationType::CORONAL){
						offset[2] = (float)(actual_height / 2.0 - actual_height * clip / 2.0);
					}
					camera->Move(offset);
				}

				// ����һ������ƽ�棬ƽ�淨�߷���ָ���и�ķ���
				vtkSmartPointer<vtkPlane> plane1 = vtkSmartPointer<vtkPlane>::New();//����һ��ƽ��
				plane1->SetOrigin(0.0, 0.0, image_count * image_spacings[2] * (1.0f - clip));	//��һ�����ϵĵ�
				plane1->SetNormal(0.0, 0.0, -1.0);	//������ķ�����

				// ����һ������Ķ���
				vtkSmartPointer<vtkPlaneCollection> planes = vtkSmartPointer<vtkPlaneCollection>::New();
				planes->AddItem(plane1);

				vol_renderer->SetClippingPlanes(planes);
			}
			else{
				vol_renderer->SetClippingPlanes(NULL);
			}

			float count_angle = 0.0f;
			int i;
			for(i=0; i<image_number; ++i){
				CGLogger::Info("do render " + to_string(i));

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
					string path = output_path + "batch_" + to_string(count_angle) + ".bmp";
					BufferResult *result = new BufferResult();
					result->buffer_data = buffer;
					result->image_plane = plane;
					result->instance_number = i + 1;
					result->file_name = path;
					result->step = step_angle;
					this->GetCallback()->OnBatchOneGenerated(result, true);
				}

				CGLogger::Info("do render end " + to_string(i));

				// �ۼӽǶ�
				count_angle += step_angle;
			}

			if (this->GetCallback()){
				this->GetCallback()->OnBatchAllGenerated(1);
			}
		}
		catch (exception* e)
		{
			CGLogger::Error("VRRotationBatchGenerator::Execute error.", e->what());
		}
	}
	else{
		if (this->callback_){
			this->callback_->OnBatchAllGenerated(0);
		}
	}
}

void VRRotationBatchGenerator::Destroy()
{

}