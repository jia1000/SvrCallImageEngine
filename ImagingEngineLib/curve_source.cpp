/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_source.cpp
  author: 	 zhangjian
  Brief:	 

=========================================================================*/
#include "curve_source.h"

using namespace DW;
using namespace DW::IMAGE;


CurveSource *CurveSource::instance_ = 0;
CurveSource *CurveSource::Get() 
{
	if (instance_ == nullptr)
		instance_ = new CurveSource;
	return instance_;
}

VolCurve *CurveSource::GetCurveById(string series_uid, string curve_id)
{
	if (curve_list_.count(series_uid) == 0){
		return NULL;
	}
	vector<VolCurve *> curves_in_series = curve_list_[series_uid];
	auto it = curves_in_series.begin();
	while (it != curves_in_series.end()){	

		if ((*it)->GetCurveID() == curve_id){
			return (*it);
		}

		++it;
	}
	return NULL;
}

VolCurve *CurveSource::GetCurveByName(string series_uid, string curve_name)
{
	if (curve_list_.count(series_uid) == 0){
		return NULL;
	}
	vector<VolCurve *> curves_in_series = curve_list_[series_uid];
	auto it = curves_in_series.begin();
	while (it != curves_in_series.end()){	

		if ((*it)->GetCurveName() == curve_name){
			return (*it);
		}

		++it;
	}
	return NULL;
}

string CurveSource::CreateCurve(string series_uid, string curve_name, vector<Point3f> points, double spacings[3])
{
	// Check if the key exists in curve_list_, insert one if not
	if (curve_list_.count(series_uid) == 0){
		vector<VolCurve *> vec;
		curve_list_.insert(make_pair(series_uid, vec));
	}
	// Add control point to curve by looping the point list
	vector<VolCurve *> curves_in_series = curve_list_[series_uid];
	VolCurve *tmp_curve = new VolCurve();
	tmp_curve->SetCurveName(curve_name);
	auto it = points.begin();
	while (it != points.end()){		
		tmp_curve->AddControlPoint((*it).x, (*it).y, (*it).z);		
		++it;
	}
	// Set voxel spacing to calculate curve length
	tmp_curve->SetVoxelSpacing(spacings);
	// Generate sample points
	tmp_curve->Update();
	// Add to cache
	curve_list_[series_uid].push_back(tmp_curve);

	return tmp_curve->GetCurveID();
}

void CurveSource::Destroy(string series_uid, string id)
{
	//TODO
}

void CurveSource::DestroyAll(string series_uid)
{
	if (curve_list_.count(series_uid) == 0){
		return;
	}
	vector<VolCurve *> curves_in_series = curve_list_[series_uid];
	auto it = curves_in_series.begin();
	while (it != curves_in_series.end()){	

		delete (*it);
		++it;
	}
	curves_in_series.clear();
	curve_list_.erase(series_uid);
}

void CurveSource::DestroyAll()
{
	//TODO
}