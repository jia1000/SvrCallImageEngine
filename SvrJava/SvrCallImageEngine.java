class SvrCallImageEngine {
	static {
		System.loadLibrary("SvrCallImageEngine");
	}
	private native void print();
	private native int test_int1(int n);
	private native float test_double1(double f);
	private native String test_string1(String s);
	private native int test_int_array(int[] intArray);
	private native String test_json(String str_json_data);
	private native int LoadSeries(String str_json_data);
	private native int SwitchSeries(String str_json_data);
	private native int UnloadSeries(String str_json_data);
	private native int ProcessSeries(String str_json_data);

	public static void main(String args[]) {
		System.out.println("begin...............................................");

        	//new SvrCallImageEngine().print();
		//new SvrCallImageEngine().test_int1(5);
		//new SvrCallImageEngine().test_double1(1.1);
		//new SvrCallImageEngine().test_string1("testonly");

		//int[] array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 101};
		//new SvrCallImageEngine().test_int_array(array);
		{
			String json_data = "{";
			json_data += "\"dicom_path\":\"../build/\",";
			json_data += "\"mask_path\":\"../build/\",";
			json_data += "\"curve_path\":\"../build/\",";
			json_data += "\"patient_id\":\"zhangsan\",";
			json_data += "\"study_uid\":\"111\",";
			json_data += "\"series_uid\":\"222\"";
			json_data += "}";
			new SvrCallImageEngine().LoadSeries(json_data);
		}
		{			
			String json_data = "{";
			json_data += "\"patient_id\":\"lilsi\",";
			json_data += "\"study_uid\":\"333\",";
			json_data += "\"series_uid\":\"444\"";
			json_data += "}";
			new SvrCallImageEngine().SwitchSeries(json_data);
		}
		{
			String json_data = "{";
			json_data += "\"patient_id\":\"wangmazi\",";
			json_data += "\"study_uid\":\"555\",";
			json_data += "\"series_uid\":\"666\"";
			json_data += "}";
			new SvrCallImageEngine().UnloadSeries(json_data);
		}
		{
			//json_data += "\"image_type\":0,";
			//json_data += "\"vessel_name\":\"L_ICA\",";
			
			// CPR 操作
			String json_data = "{";
			json_data += "\"image_type\":2,"; 				// 图像类型
			json_data += "\"vessel_name\":\"L_ICA\","; 		// 血管名称
			json_data += "\"Init_orientation\":2,"; 		// 正位（起始位置）标识
			json_data += "\"Rotation_direction\":0,"; 		// 旋转方向
			json_data += "\"rotation_angle\":45.0,"; 		// 旋转角度
			json_data += "\"Output_Image_number\":4,"; 	// 输出图像数目
			json_data += "\"Output_path\":\"../build/cpr/\","; // 输出图像目录
			json_data += "\"Window_width\":2000,"; 		// 窗宽
			json_data += "\"Window_level\":400,"; 			// 窗位
			json_data += "\"Last_variable\":999"; 			// 
			json_data += "}";
			new SvrCallImageEngine().ProcessSeries(json_data);
		}
		{
			// VR 和 VRMIP 操作
			String json_data = "{";
			json_data += "\"image_type\":0,"; 				// 图像类型
			json_data += "\"Blend_mode\":0,"; 				// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"Init_orientation\":2,"; 		// 正位（起始位置）标识
			json_data += "\"Clip_percent\":1.0,"; 			// 截取比例;  若为0.35，则表示颅内;其它为完整
			json_data += "\"Rotation_direction\":0,"; 		// 旋转方向
			json_data += "\"rotation_angle\":30.0,"; 		// 旋转角度
			json_data += "\"Output_Image_number\":12,"; 	// 输出图像数目
			json_data += "\"Output_path\":\"../build/vr/\","; // 输出图像目录
			json_data += "\"Window_width\":2000,"; 		// 窗宽
			json_data += "\"Window_level\":400,"; 			// 窗位
			json_data += "\"Last_variable\":999"; 			// 
			json_data += "}";
			new SvrCallImageEngine().ProcessSeries(json_data);
		}
		{
			// MPRMIP 操作
			String json_data = "{";
			json_data += "\"image_type\":1,"; 				// 图像类型
			json_data += "\"Blend_mode\":1,"; 				// 绘制方式 : VR为0；  VRMIP为1
			json_data += "\"Init_orientation\":0,"; 		// 正位（起始位置）标识
			json_data += "\"Clip_percent\":0.35,"; 		// 截取比例;  
			json_data += "\"thickness\":25.0,"; 			// 层厚
			json_data += "\"Spacing_between_slices\":5.0,";// 层间距
			json_data += "\"Output_path\":\"../build/mprmip/\","; // 输出图像目录
			json_data += "\"Window_width\":2000,"; 		// 窗宽
			json_data += "\"Window_level\":400,"; 			// 窗位
			json_data += "\"Last_variable\":999"; 			// 
			json_data += "}";
			new SvrCallImageEngine().ProcessSeries(json_data);
		}
        		System.out.println("end..................................................");
	}
}
