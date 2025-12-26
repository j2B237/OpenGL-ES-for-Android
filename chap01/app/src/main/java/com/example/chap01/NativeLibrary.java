package com.example.chap01;

public class NativeLibrary 
{

	static 
	{
		System.loadLibrary("native-lib");
	}

	// static native function on JNI side use jclass param
	// whereas non-static function on JNI side use jobject param
	public static native void init();
}