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
	public static void main(String args[]) {
		System.out.println("begin...............................................");

        	//new SvrCallImageEngine().print();
		//new SvrCallImageEngine().test_int1(5);
		//new SvrCallImageEngine().test_double1(1.1);
		//new SvrCallImageEngine().test_string1("testonly");

		//int[] array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 101};
		//new SvrCallImageEngine().test_int_array(array);

		String json_data = "{\"request_type\":\"mpr\",\"image_operation\":\"move\",\"image_paras\":\"0.9\"}";
		new SvrCallImageEngine().test_json(json_data);

        	System.out.println("end..................................................");
	}
}
