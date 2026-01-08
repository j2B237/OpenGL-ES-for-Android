package com.example.lighting;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import android.content.res.AssetManager;

public class MainActivity extends Activity
{
	private static final String TAG = "Lighting-Main";
	protected SurfaceView graphicView;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(TAG, "On Method create called");
		graphicView = new SurfaceView(getApplication());
		setContentView(graphicView);
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		graphicView.onResume();
		Log.i(TAG, "On Method resume called");
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		graphicView.onPause();
		Log.i(TAG, "On Method pause called");
	}
}