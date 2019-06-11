/****************************************************************************************************

 * \file global_define.h
 * \date 2019/06/05 11:18
 *
 * \author jiayf
 * Contact: @deepwise.com
 *
 * \brief  

 Copyright(c) 2018-2030 DeepWise Corporation

****************************************************************************************************/

#pragma once
//#define USE_JSONCPP   1
#define JSON_KEY_DICOM_PATH				"dicom_path"
#define JSON_KEY_MASK_PATH				"mask_path"
#define JSON_KEY_CURVE_PATH				"curve_path"
#define JSON_KEY_PATIENT_ID				"patient_id"
#define JSON_KEY_STUDY_UID				"study_uid"
#define JSON_KEY_SERIRES_UID			"series_uid"

#define JSON_KEY_IMAGE_TYPE 			"image_type"
#define JSON_KEY_VESSEL_NAME			"vessel_name"
#define JSON_KEY_BLEND_MODE				"blend_mode"
#define JSON_KEY_GENERATE_RULE			"generate_rule"
#define JSON_KEY_INIT_ORIENTATION		"init_orientation"
#define JSON_KEY_CLIP_PERCENT			"clip_percent"
#define JSON_KEY_THICKNESS				"thickness"
#define JSON_KEY_SPACING_BETWEEN_SLICES	"spacing_between_slices"
#define JSON_KEY_OUTPUT_IMAGE_NUMBER	"output_image_number"
#define JSON_KEY_OUTPUT_PATH			"output_path"
#define JSON_KEY_WINDOW_WIDTH			"window_width"
#define JSON_KEY_WINDOW_LEVEL			"window_level"
#define JSON_KEY_ROTATION_DIRECTION		"rotation_direction"

#define  RET_STATUS_JSON_PARSE_FAIL			(-4)
#define  RET_STATUS_JSON_VALUE_TYPE_ERROR	(-3)
#define  RET_STATUS_JSON_NAME_NOT_FOUND		(-2)
#define  RET_STATUS_FAILURE    				(-1)
#define  RET_STATUS_SUCCESS    				(1)

//  第一级 "请求类型"    的枚举
#define JSON_VALUE_REQUEST_TYPE_VR			0
#define JSON_VALUE_REQUEST_TYPE_MPR			1
#define JSON_VALUE_REQUEST_TYPE_CPR			2
