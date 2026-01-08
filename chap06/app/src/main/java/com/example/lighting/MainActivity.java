package com.example.lighting;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import android.content.res.AssetManager;
import android.widget.TextView;

public class MainActivity extends Activity
{
	private static final String TAG = "Lighting-Main";
	protected GraphicsView graphicView;
	protected TextView speed;


	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		NativeLibrary.setAssetManager(getAssets());

		setContentView(R.layout.my_layout);

        graphicView = findViewById(R.id.glView);
        speed = findViewById(R.id.hud);
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