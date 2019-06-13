/*=========================================================================

  Program:   ImagingEngine
  Module:    configurations.h
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "tools/function.h"

class IConfigReader
{
public:
	/// VR��Ⱦ����������ֵRAYCASTING, RAYCASTING_GPU...
	virtual string GetVRRenderMethod() = 0;
	/// MPR��Ⱦ����������ֵCPU, VTK_CPU
	virtual string GetMPRRenderMethod() = 0;
	/// ����������̵߳���Ŀ��������������Զ�ȡCPU�߼�������
	virtual int GetNumberOfThreads() = 0;
	/// VR / MIP
	virtual string GetVRBlendMode() = 0;
};

class ConfigurationManager
{
public:
	~ConfigurationManager() { instance_ = nullptr; }

	static ConfigurationManager* Get() 
	{
		if (instance_ == nullptr)
			instance_ = new ConfigurationManager;
		return instance_;
	}

	void SetConfigReader(IConfigReader *reader);

	string GetVRRenderMethod()
	{
		return vr_render_method_;
	}

	string GetMPRRenderMethod()
	{
		return mpr_render_method_;
	}

	int GetNumberOfThreads()
	{
		return threads_number_;
	}

	string GetVRBlendMode()
	{
		return vr_blend_mode_;
	}

private:
	ConfigurationManager();

private:
	static ConfigurationManager* instance_;
	IConfigReader *config_reader_;
	string vr_render_method_;
	string mpr_render_method_;
	int threads_number_;
	string vr_blend_mode_;

};