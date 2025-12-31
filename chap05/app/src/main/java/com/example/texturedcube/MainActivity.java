package com.example.texturedcube;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity
{
	/*[Members variables]*/
	private static String LOG_TAG = "Textured Cube";

	/*[Class Methods]*/
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.i(LOG_TAG, "On Method create");
	}

	@Override
	protected void onResume()
	{
		super.onResume();
	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}
}