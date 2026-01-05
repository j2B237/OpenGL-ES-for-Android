package com.example.lighting;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;



pulic class MainActivity extends Activity
{
	private static final String TAG = "Lighting-Main";

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG, "On Method create called");
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		Log.i(TAG, "On Method resume called");
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		Log.i(TAG, "On Method pause called");
	}
}