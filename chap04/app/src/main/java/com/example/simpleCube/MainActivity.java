// The first line should be included in all of your Java source files.
// This tells your application which package this Java file belongs too.
package com.example.simpleCube;

// As this class is going to be the starting point of your activity 
// you need to import the Activity class.
import android.app.Activity;
import android.os.Bundle;

// the logging import helps with debugging in your application. 
// It enables printf like debugging in your Android application.
import android.util.Log;

// As we don't want to start our class from scratch 
// we say that it extends the Activity class. Which indicates to Android 
// this is the starting class.

public class MainActivity extends Activity 
{

	// Member variable
	private static String LOGTAG = "Simple Triangle";

	protected TutorialView graphicsView;

	/*
		This is the first method in our class and it is also the first function 
		that gets called in our application. As we are not interested in most of the setup ourself 
		we call the onCreate method of the super class. We then add our first debug statement. 
		This will output the string 'On Create method Calling native Library' which will be visible in LogCat.
	*/

	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);

		// Logcat is the Android logging tool and it is very useful.
		Log.d(LOGTAG, "On Create Method Calling Native Library");
		graphicsView = new TutorialView(getApplication());
		setContentView(graphicsView);
	}

	@Override
	protected void onPause()
	{
		super.onPause();
		graphicsView.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
		graphicsView.onResume();
	}

}