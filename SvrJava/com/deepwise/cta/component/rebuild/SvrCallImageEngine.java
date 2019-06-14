package com.deepwise.cta.component.rebuild;

public class SvrCallImageEngine {
	static {
		// charls库是jpeg-ls的编解码库
		System.loadLibrary("charls");		
		// System.loadLibrary("ImagingEngineLib");
		System.loadLibrary("SvrCallImageEngine");
	}
	
	private native int loadSeries(String str_json_data);
	private native int switchSeries(String str_json_data);
	private native int unloadSeries(String str_json_data);
	private native int processSeries(String str_json_data);

	public static void main(String args[]) {
		System.out.println("begin...............................................");
		boolean opened = true;
        // 加载Series
		if(opened)
		{
			String json_data = "{";
			json_data += "\"dicom_path\":\"../dicom_source/\",";
			json_data += "\"mask_path\":\"../mask_source/\",";
			json_data += "\"curve_path\":\"../curve_source/\",";
			json_data += "\"patient_id\":\"zhangsan\",";
			json_data += "\"study_uid\":\"111\",";
			json_data += "\"series_uid\":\"222\"";
			json_data += "}";
			new SvrCallImageEngine().loadSeries(json_data);
		}
		// 切换Series
		if(!opened)
		{			
			String json_data = "{";
			json_data += "\"patient_id\":\"lilsi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += "\"series_uid\":\"444\"";
			json_data += "}";
			new SvrCallImageEngine().switchSeries(json_data);
		}
		// 卸载Series
		if(!opened)
		{
			String json_data = "{";
			json_data += "\"patient_id\":\"wangmazi\",";
			json_data += "\"study_uid\":\"555\",";
			json_data += "\"series_uid\":\"666\"";
			json_data += "}";
			new SvrCallImageEngine().unloadSeries(json_data);
		}
		
		if(!opened)
		{
			// CPR 操作
			String json_data = "{";
			json_data += "\"image_type\":2,"; 			// 图像类型
			json_data += "\"vessel_name\":\"R_ICA\","; 	// 血管名称
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":45.0,"; 	// 旋转角度
			json_data += "\"output_image_number\":4,"; 	// 输出图像数目
			json_data += "\"output_path\":\"../build/cpr7/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":777"; 		// 
			json_data += "}";
			new SvrCallImageEngine().processSeries(json_data);
		}
		if(opened)
		{
			// VR 和 VRMIP 操作
			String json_data = "{";
			json_data += "\"image_type\":0,"; 			// 图像类型
			json_data += "\"blend_mode\":0,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
			json_data += "\"generate_rule\":0,"; 		// generate_rule
			json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"rotation_direction\":0,"; 	// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
			json_data += "\"output_image_number\":12,";	// 输出图像数目
			json_data += "\"output_path\":\"../build/vr8/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":888"; 		// 
			json_data += "}";
			new SvrCallImageEngine().processSeries(json_data);
		}
		if(!opened)
		{
			// MPRMIP 操作
			String json_data = "{";
			json_data += "\"image_type\":1,"; 			// 图像类型
			json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"init_orientation\":0,"; 	// 正位（起始位置）标识
			json_data += "\"clip_percent\":0.35,"; 		// 截取比例;  
			json_data += "\"thickness\":25.0,"; 		// 层厚
			json_data += "\"spacing_between_slices\":5.0,";// 层间距
			json_data += "\"output_path\":\"../build/mprmip9/\","; // 输出图像目录
			json_data += "\"window_width\":2000,"; 		// 窗宽
			json_data += "\"window_level\":400,"; 		// 窗位
			json_data += "\"last_variable\":999"; 		// 
			json_data += "}";
			new SvrCallImageEngine().processSeries(json_data);
		}
        System.out.println("end..................................................");
	}
}
