package com.example.lighting;

public class NativeLibrary
{
	// Android Load Native C++ fonctionnality
	static
	{
		System.loadLibrary("native-lib");
	}	

	// Define function signatures in Java which should match
	// NDK functions in c++
	public static native void initGraphics(int width, int height);
	public static native void update(int width, int height);
	public static native void render();
	public static native void loadResources();
}