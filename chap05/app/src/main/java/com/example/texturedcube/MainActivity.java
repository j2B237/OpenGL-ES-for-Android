package com.example.texturedcube;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity
{
	/*[Members variables]*/
	private static String LOG_TAG = "Textured Cube";
	protected TutorialView graphicsView;

	/*[Class Methods]*/
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(LOG_TAG, "On Method create");
		graphicsView = new TutorialView(getApplication());
		setContentView(graphicsView);
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		graphicsView.onResume();
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		graphicsView.onPause();
	}
}