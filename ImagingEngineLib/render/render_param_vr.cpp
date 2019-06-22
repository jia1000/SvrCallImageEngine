#include "render/render_param_vr.h"

using namespace DW::IMAGE;
using namespace DW::Render;

VRRenderParam::VRRenderParam()
{
	// size
	width_ = 512;
	height_ = 512;
	scale_factor_ = 1.0f;
	//// camera
	//camera_ = new Camera();
	//// light
	//light_ = new Light();
	// set default desired update rate
	desired_update_rate_ = 0.01;
	// set default render mode
	rendering_mode_ = RenderMode::RAYCASTING;
	update_center_after_clipped_ = false;
	// 1.0 means no clipping
	clipping_percent_ = 1.0;
}

VRRenderParam::~VRRenderParam()
{
	
}

void VRRenderParam::SetDesiredUpdateRate(float rate)
{
	desired_update_rate_ = rate;
}

float VRRenderParam::GetDesiredUpdateRate()
{
	return desired_update_rate_;
}

void VRRenderParam::SetRenderingMode(RenderMode mode)
{
	rendering_mode_ = mode;
}

RenderMode VRRenderParam::GetRenderMode()
{
	return rendering_mode_;
}

void VRRenderParam::SetBlendMode(BlendMode mode)
{
	blend_mode_ = mode;
}

BlendMode VRRenderParam::GetBlendMode()
{
	return blend_mode_;
}

void VRRenderParam::SetUpdateCenterAfterClipping(bool update)
{
	update_center_after_clipped_ = update;
}

bool VRRenderParam::GetUpdateCenterAfterClipping()
{
	return update_center_after_clipped_;
}

void VRRenderParam::SetClipping(float clip)
{
	clipping_percent_ = clip;
}

float VRRenderParam::GetClipping()
{
	return clipping_percent_;
}