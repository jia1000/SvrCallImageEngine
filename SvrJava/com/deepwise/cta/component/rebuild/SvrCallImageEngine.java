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
		int MAX_TEST = 2;
		String[] series_uid = {
			"\"series_uid\":\"1\"",
			"\"series_uid\":\"2\""
		};
		
		for (int i = 0; i < MAX_TEST; i++) {	
			// 加载Series
			if(opened)
			{
				String[] dicom_path = {
					"\"dicom_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/dicom_file/\",",
					"\"dicom_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source2/dicom_file/\","
				};
				String[] mask_path = {
					"\"mask_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/mask_source/\",",
					"\"mask_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source2/mask_source/\","
				};
				String[] curve_path = {
					"\"curve_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/curve_source/\",",
					"\"curve_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source2/curve_source/\","
				};
				String json_data = "{";
				json_data += dicom_path[i];//"\"dicom_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_source/\",";
				json_data += mask_path[i];//"\"mask_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/dicom_sourcemask_source/\",";
				json_data += curve_path[i];//"\"curve_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/curve_source/\",";
				json_data += "\"patient_id\":\"zhangsan\",";
				json_data += "\"study_uid\":\"111\",";
				json_data += series_uid[i];//"\"series_uid\":\"222\"";
				json_data += "}";
				new SvrCallImageEngine().loadSeries(json_data);
			}
			
			
			// 切换Series
			if(opened)
			{			
				String json_data = "{";
				json_data += "\"patient_id\":\"lilsi\",";
				json_data += "\"study_uid\":\"333\",";
				json_data += series_uid[i];//"\"series_uid\":\"444\"";
				json_data += "}";
				new SvrCallImageEngine().switchSeries(json_data);
			}			

			if(opened)
			{
				String[] output_path = {
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/vr1/\",",
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/vr2/\","
				};
				String[] output_image_number = {
					"\"output_image_number\":12,",
					"\"output_image_number\":16,"
				};
				// VR 和 VRMIP 操作
				String json_data = "{";
				json_data += "\"image_type\":0,"; 			// 图像类型
				json_data += "\"blend_mode\":0,"; 			// 绘制方式 : VR为0；  VRMIP为1
				json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
				json_data += "\"generate_rule\":0,"; 		// generate_rule
				json_data += "\"clip_percent\":1.0,"; 		// 截取比例;  若为0.35，则表示颅内;其它为完整
				json_data += "\"rotation_direction\":0,"; 	// 旋转方向
				json_data += "\"rotation_angle\":30.0,"; 	// 旋转角度
				json_data += output_image_number[i];//"\"output_image_number\":12,";	// 输出图像数目
				json_data += output_path[i];//"\"output_path\":\"../build/vr8/\","; // 输出图像目录
				json_data += "\"window_width\":2000,"; 		// 窗宽
				json_data += "\"window_level\":400,"; 		// 窗位
				json_data += "\"last_variable\":888"; 		// 
				json_data += "}";
				new SvrCallImageEngine().processSeries(json_data);
			}


			if(opened)
			{
				String[] output_path = {
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/mpr1/\",",
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/mpr2/\","
				};
				// MPRMIP 操作
				String json_data = "{";
				json_data += "\"image_type\":1,"; 			// 图像类型
				json_data += "\"blend_mode\":1,"; 			// 绘制方式 : VR为0；  VRMIP为1
				json_data += "\"init_orientation\":0,"; 	// 正位（起始位置）标识
				json_data += "\"clip_percent\":0.35,"; 		// 截取比例;  
				json_data += "\"thickness\":25.0,"; 		// 层厚
				json_data += "\"spacing_between_slices\":5.0,";// 层间距
				json_data += output_path[i];//"\"output_path\":\"../build/mprmip2/\","; // 输出图像目录
				json_data += "\"window_width\":2000,"; 		// 窗宽
				json_data += "\"window_level\":400,"; 		// 窗位
				json_data += "\"last_variable\":999"; 		// 
				json_data += "}";
				new SvrCallImageEngine().processSeries(json_data);
			}

			if(opened)
			{
				String[] output_path = {
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/cpr1/\",",
					"\"output_path\":\"/home/clientdemo/SvrCallImageEngineGit/SvrCallImageEngine/build/cpr2/\","
				};
				String[] output_image_number = {
					"\"output_image_number\":12,",
					"\"output_image_number\":16,"
				};
				// CPR 操作
				String json_data = "{";
				json_data += "\"image_type\":2,"; 			// 图像类型
				json_data += "\"vessel_name\":\"R_ICA\","; 	// 血管名称
				json_data += "\"init_orientation\":2,"; 	// 正位（起始位置）标识
				json_data += "\"rotation_direction\":0,"; 	// 旋转方向
				json_data += "\"rotation_angle\":45.0,"; 	// 旋转角度
				json_data += output_image_number[i];//"\"output_image_number\":4,"; 	// 输出图像数目
				json_data += output_path[i];//"\"output_path\":\"../build/cpr7/\","; // 输出图像目录
				json_data += "\"window_width\":2000,"; 		// 窗宽
				json_data += "\"window_level\":400,"; 		// 窗位
				json_data += "\"last_variable\":777"; 		// 
				json_data += "}";
				new SvrCallImageEngine().processSeries(json_data);
			}

			// 卸载Series
			if(opened)
			{
				String json_data = "{";
				json_data += "\"patient_id\":\"wangmazi\",";
				json_data += "\"study_uid\":\"333\",";
				json_data += series_uid[i];//"\"series_uid\":\"666\"";
				json_data += "}";
				new SvrCallImageEngine().unloadSeries(json_data);
			}
		}
		System.out.println("end..................................................");
	}
}
