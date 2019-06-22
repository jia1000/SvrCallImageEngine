/*=========================================================================

  Program:   ImagingEngine
  Module:    render_facade.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "render_facade.h"
#include "render/render_param.h"
#include "render/render_param_mpr.h"
#include "render/render_param_cpr.h"
#include "render/render_param_vr.h"
#include "curve_source.h"
#include "data_source.h"
#include "batch/batch_generator_vr.h"
#include "batch/batch_generator_cpr.h"
#include "batch/batch_generator_mpr.h"
#include "batch/batch_image_source.h"
#include "control/image_control.h"
#include "control/image_control_2d.h"
#include "control/image_control_3d.h"
#include "tools/timer.h"
#include "tools/logger.h"
#include "exception/iexception.h"
#include "exception/data_exception.h"

using namespace DW;
using namespace DW::IMAGE;
using namespace DW::Render;
using namespace DW::Batch;
using namespace DW::Control;


RenderFacade *RenderFacade::instance_ = 0;
RenderFacade *RenderFacade::Get() 
{
	if (instance_ == nullptr)
		instance_ = new RenderFacade;
	return instance_;
}

IImageControl *RenderFacade::GetControl(string control_id)
{
	return RenderSource::Get()->GetImageControl(control_id);
}

IBitmap *RenderFacade::GetImageBuffer(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return NULL;

	return control->GetOutput();
}

void RenderFacade::Zoom(string control_id, float scale)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Zoom(scale);
	control->Update();
}

void RenderFacade::ZoomToSize(string control_id, int width, int height)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	//TODO update width and height...or redefine the interface...
	control->ZoomToSize(width, height);
	control->Update();
}

void RenderFacade::Pan(string control_id, float dx, float dy)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Pan(dx, dy);
	control->Update();
}

void RenderFacade::MoveTo(string control_id, float move_vector[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Move(move_vector);
	control->Update();
}

void RenderFacade::Roll(string control_id, float angle)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Roll(angle);
	control->Update();
}

void RenderFacade::Rotate(string control_id, float angle, float axis[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	Vector3f axis_vec(axis[0], axis[1], axis[2]);
	control->Rotate3D(axis_vec, angle);
	control->Update();
}

void RenderFacade::SetOrientation(string control_id, OrientationType ori)
{
	IImageControl *control = GetControl(control_id);
	if (NULL == control) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	RenderMode mode = imaging->GetRenderMode();
	switch (mode)
	{
	case DW::IMAGE::RenderMode::SMART_RAYCASTING:
	case DW::IMAGE::RenderMode::RAYCASTING_GPU:
	case DW::IMAGE::RenderMode::RAYCASTING:
	case DW::IMAGE::RenderMode::TEXTURE_3D_GPU:
	case DW::IMAGE::RenderMode::ISO_SURFACE:
	case DW::IMAGE::RenderMode::SSD:
		{
			Camera *camera = imaging->GetRenderer()->GetCamera();
			if (camera){
				camera->SetCameraViewPlane(ori);
			}
		}
		break;
	case DW::IMAGE::RenderMode::MPR:
	case DW::IMAGE::RenderMode::CROSS_SECTIONAL_MPR:
		{
			MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_mpr){
				Vector3f row_vec, col_vec;
				//Point3f image_center;
				if (ori == OrientationType::AXIAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					row_vec[0] = 0.0f;
					row_vec[1] = 1.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else if (ori == OrientationType::CORONAL){
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 0.0f;
					col_vec[2] = -1.0f;
				}
				else{
					row_vec[0] = 1.0f;
					row_vec[1] = 0.0f;
					row_vec[2] = 0.0f;
					col_vec[0] = 0.0f;
					col_vec[1] = 1.0f;
					col_vec[2] = 0.0f;
				}
				param_mpr->SetPlaneVector(row_vec, col_vec);
			}
		}
		break;
	case DW::IMAGE::RenderMode::STRAIGHTENED_CPR:
	case DW::IMAGE::RenderMode::STRETECHED_CPR:
		{
			CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
			if (param_cpr){
				Vector3f vec_of_interest;
				if (ori == OrientationType::AXIAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}
				else if (ori == OrientationType::SAGITTAL){
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 1.0f;
					vec_of_interest[2] = 0.0f;
				}
				else if (ori == OrientationType::CORONAL){
					vec_of_interest[0] = 1.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = 0.0f;
				}
				else{
					vec_of_interest[0] = 0.0f;
					vec_of_interest[1] = 0.0f;
					vec_of_interest[2] = -1.0f;
				}

				param_cpr->SetDirection(vec_of_interest[0], vec_of_interest[1], vec_of_interest[2]);
			}
		}
		break;
	default:
		break;
	}
}

void RenderFacade::SetOrientation(string control_id, OrientationType ori, float vec[3])
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	//TODO finish set orientation
}

void RenderFacade::SetCPRCurveID(string control_id, string curve_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;
	
	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (NULL == imaging) return;

	CPRRenderParam *param_cpr = dynamic_cast<CPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_cpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurveById(current_series_uid_, curve_id);
	if (curve == NULL) return;

	param_cpr->SetCurve(curve);
}

void RenderFacade::SetPositionAlongCurve(string control_id, string curve_id, int point_index)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (NULL == imaging) return;

	MPRRenderParam *param_mpr = dynamic_cast<MPRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_mpr == NULL) return;

	VolCurve *curve = CurveSource::Get()->GetCurveById(current_series_uid_, curve_id);
	if (curve == NULL) return;

	Vector3f vx, vy;
	float ix, iy, iz;
	// Validate point_index is between min/max value
	int number_of_components = curve->GetNumberOfSamplePoint();
	if (point_index >= number_of_components){
		point_index = number_of_components - 1;
	}
	else if (point_index < 0){
		point_index = 0;
	}
	curve->GetSamplePoint(point_index, ix, iy, iz);
	curve->GetLocalPlaneVector(point_index, vx, vy);

	// Set imaging parameters
	param_mpr->SetImageCenter(ix, iy, iz);
	param_mpr->SetPlaneVector(vx, vy);

	imaging->Render();
}

void RenderFacade::SetDesiredUpdateRate(string control_id, float rate)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	VRRenderParam *param_vr = dynamic_cast<VRRenderParam *>(imaging->GetRenderer()->GetRenderParam());
	if (param_vr == NULL) return;

	param_vr->SetDesiredUpdateRate(rate);
}

void RenderFacade::RenderControl(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	control->Update();
}

void RenderFacade::ResetControl(string control_id)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	//TODO reset control
}

void RenderFacade::ChangeSeries(string series_uid)
{
	current_series_uid_ = series_uid;
	VolData *data = ImageDataSource::Get()->GetVolData(series_uid);
	if (data){
		vector<IThreedImaging *> imaging_list = RenderSource::Get()->GetRenderControls();
		auto it = imaging_list.begin();
		for (; it!=imaging_list.end(); ++it){
			(*it)->SetData(data);
			RenderParam *param = (*it)->GetRenderer()->GetRenderParam();
			if (param){
				// Set default window width & level
				int width, level;
				data->GetDefaultWindowWidthLevel(width, level);
				param->SetWindowWidthLevel(width, level);
			}
			(*it)->GetRenderer()->SetData(data);
		}
	}
}

void RenderFacade::UnloadSeries(string series_uid)
{
	if (current_series_uid_ == series_uid){
		current_series_uid_ = "";
		ImageDataSource::Get()->Destroy(series_uid);

		vector<IThreedImaging *> imaging_list = RenderSource::Get()->GetRenderControls();
		auto it = imaging_list.begin();
		for (; it!=imaging_list.end(); ++it){
			(*it)->SetData(NULL);
			(*it)->GetRenderer()->SetData(NULL);
		}
	}
}

void RenderFacade::SetImageSize(string control_id, int width, int height)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	//TODO 将此处代码移植到control内
	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	//TODO control设置display width & height
	param->SetWidth(width);
	param->SetHeight(height);
}

void RenderFacade::SetWindowWidthLevel(string control_id, int width, int level)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;
	
	control->WWWL(width, level);
	control->Update();
}

void RenderFacade::SetBlendMode(string control_id, int mode)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(0));
	if (NULL == imaging) return;

	RenderParam *param = imaging->GetRenderer()->GetRenderParam();
	if (param == NULL) return;

	if (mode == 1){
		param->SetBlendMode(BlendMode::MaximumIntensity);
	}
	else if (mode == 2){
		param->SetBlendMode(BlendMode::MinimumIntensity);
	}
	else{
		param->SetBlendMode(BlendMode::AverageIntensity);
	}
}

void RenderFacade::SetSlabThickness(string control_id, float thickness)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return;


}

int RenderFacade::CreateVRRotationBatch(string control_id, 
						   string output, 
						   BlendMode mode, 
						   OrientationType init_ori, 
						   RotationDirection direction, 
						   float step_angle, 
						   float clip_percent,
						   int image_number,
						   int ww,
						   int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return -1;

	try
	{

		Timer::begin("CreateVRRotationBatch");

		CGLogger::Info("RenderFacade::CreateVRRotationBatch 1");

		// Create batch image source
		BatchImageSource *source = new BatchImageSource();
		source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
		control->SetInput(0, source);

		CGLogger::Info("RenderFacade::CreateVRRotationBatch 2");

		IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
		if (imaging){
			VRRotationBatchGenerator generator;
			generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
			VRRotationBatchArgs args;
			args.SetOutputPath(output);
			args.SetBlendMode(mode);
			args.SetOrientation(init_ori);
			args.SetImageNumber(image_number);
			args.SetStep(step_angle);
			args.SetDirection(direction);
			args.SetClipPercent(clip_percent);
			args.SetWWWL(ww, wl);
			args.SetImaging(imaging);

			CGLogger::Info("RenderFacade::CreateVRRotationBatch 3");

			generator.Execute(&args);

			CGLogger::Info("RenderFacade::CreateVRRotationBatch 4");

			Timer::begin("CreateVRRotationBatch::SaveAsBitmap");
			// create files on disk
			control->SaveAsBitmap(output);
			Timer::end("CreateVRRotationBatch::SaveAsBitmap");
		}

		Timer::end("CreateVRRotationBatch");
		CGLogger::Debug(Timer::summery());

	}
	catch(exception *e)
	{
		CGLogger::Error("RenderFacade::CreateVRRotationBatch error.", e->what());
	}
}

int RenderFacade::CreateCPRRotationBatch(string control_id,
										  string output,
										  string curve_id,
										  OrientationType init_ori,
										  RotationDirection direction, 
										  float step_angle, 
										  int image_number,
										  int ww,
										  int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return -1;

	Timer::begin("CreateCPRRotationBatch");

	// Create batch image source
	BatchImageSource *source = new BatchImageSource();
	source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
	control->SetInput(0, source);

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (imaging){
		CPRRotationBatchGenerator generator;
		generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
		CPRRotationBatchArgs args;
		args.SetOutputPath(output);
		args.SetOrientation(init_ori);
		args.SetImageNumber(image_number);
		args.SetStep(step_angle);
		args.SetDirection(direction);
		args.SetWWWL(ww, wl);
		args.SetCurveId(curve_id);
		args.SetImaging(imaging);
		generator.Execute(&args);

		Timer::begin("CreateCPRRotationBatch::SaveAsBitmap");
		// create files on disk
		control->SaveAsBitmap(output);
		Timer::end("CreateCPRRotationBatch::SaveAsBitmap");
	}

	Timer::end("CreateCPRRotationBatch");
	CGLogger::Debug(Timer::summery());
}

int RenderFacade::CreateMPRSlabBatch(string control_id,
									  string output,
									  BlendMode mode,
									  OrientationType ori,
									  float clip_percent, 
									  float thickness,
									  float spacing,
									  int ww,
									  int wl)
{
	IImageControl *control = GetControl(control_id);
	if (control == NULL) return -1;

	Timer::begin("CreateMPRSlabBatch");
	
	// Create batch image source
	BatchImageSource *source = new BatchImageSource();
	source->SetCallback(dynamic_cast<IImageSouceCallback *>(control));
	control->SetInput(0, source);

	IThreedImaging *imaging = reinterpret_cast<IThreedImaging *>(control->GetInput(1));
	if (imaging){
		MPRSlabBatchGenerator generator;
		generator.SetCallback(dynamic_cast<IBatchCallback *>(source));
		MPRSlabBatchArgs args;
		args.SetOutputPath(output);
		args.SetBlendMode(mode);
		args.SetOrientation(ori);
		args.SetClipPercent(clip_percent);
		//args.SetStep(spacing);
		args.SetThickness(thickness);
		args.SetSpacingBetweenSlice(spacing);
		args.SetWWWL(ww, wl);
		args.SetImaging(imaging);
		generator.Execute(&args);

		Timer::begin("CreateMPRSlabBatch::SaveAsBitmap");
		// create files on disk
		control->SaveAsBitmap(output);
		Timer::end("CreateMPRSlabBatch::SaveAsBitmap");
	}

	Timer::end("CreateMPRSlabBatch");
	CGLogger::Debug(Timer::summery());
}