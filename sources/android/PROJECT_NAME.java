/**
*******************************************************************************
* @file ewol __PROJECT_NAME__.java
* @brief Java __PROJECT_NAME__ code.
* @author Edouard DUPIN
* @date 20/04/2012
* @par Project
* ewol
*
* @par Copyright
* Copyright 2011 Edouard DUPIN, all right reserved
*
* This software is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY.
*
* Licence summary : 
*    You can modify and redistribute the sources code and binaries.
*    You can send me the bug-fix
*
* Term of the licence in in the file licence.txt.
*
*******************************************************************************
*/


package __PROJECT_ORG_TYPE__.__PROJECT_VENDOR__.__PROJECT_PACKAGE__;



import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;
// For No Title : 
import android.view.Window;

// For the full screen : 
import android.view.WindowManager;
// for the keyboard event :
import android.view.inputmethod.InputMethodManager;

import java.io.File;
import android.content.Context;
import android.content.res.Configuration;

// For the getting apk name : 
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.util.DisplayMetrics;

// import the ewol package :
import org.ewol.Ewol;
import org.ewol.EwolSurfaceViewGL;
import org.ewol.EwolAudioTask;
import org.ewol.EwolCallback;
import org.ewol.EwolConstants;

import java.io.IOException;

import static org.ewol.Ewol.EWOL;

/**
 * @brief Class : 
 *
 */
public class __PROJECT_NAME__ extends Activity implements EwolCallback, EwolConstants{

    private EwolSurfaceViewGL mGLView;
    private EwolAudioTask     mStreams;
    private Thread            mAudioThread;

    static {
	System.loadLibrary("__PROJECT_NAME__");
    }

    @Override protected void onCreate(Bundle savedInstanceState)
    {
	super.onCreate(savedInstanceState);
		
	// set the java evironement in the C sources : 
	Ewol.setJavaVirtualMachineStart(this);
		
	// Load the application directory
	Ewol.paramSetArchiveDir(1, getFilesDir().toString());
	Ewol.paramSetArchiveDir(2, getCacheDir().toString());
	// to enable extarnal storage: add in the manifest the restriction needed ...
	//packageManager.checkPermission("android.permission.READ_SMS", myPackage) == PERMISSION_GRANTED; 
	//Ewol.paramSetArchiveDir(3, getExternalCacheDir().toString());
		
	// return apk file path (or null on error)
	String apkFilePath = null;
	ApplicationInfo appInfo = null;
	PackageManager packMgmr = getPackageManager();
	try {
	    appInfo = packMgmr.getApplicationInfo("__PROJECT_ORG_TYPE__.__PROJECT_VENDOR__.__PROJECT_PACKAGE__", 0);
	} catch (NameNotFoundException e) {
	    e.printStackTrace();
	    throw new RuntimeException("Unable to locate assets, aborting...");
	}
	apkFilePath = appInfo.sourceDir;
	Ewol.paramSetArchiveDir(0, apkFilePath);
		
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        EWOL.displayPropertyMetrics(metrics.xdpi, metrics.ydpi);

	// call C init ...
	EWOL.onCreate();
		
	// Remove the title of the current display : 
	requestWindowFeature(Window.FEATURE_NO_TITLE);
	// set full screen Mode : 
	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
	// display keyboard:
	//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
	// hide keyboard : 
	getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		
	// create bsurface system
	mGLView = new EwolSurfaceViewGL(this, __CONF_OGL_ES_V__);
		
	// create element audio ...
	mStreams = new EwolAudioTask();
		
	setContentView(mGLView);
    }

    @Override protected void onStart()
    {
	super.onStart();
	// call C
	EWOL.onStart();
    }
	
    @Override protected void onRestart()
    {
	super.onRestart();
	// call C
	EWOL.onReStart();
    }
	
    @Override protected void onResume()
    {
	super.onResume();
	mGLView.onResume();
	mAudioThread = new Thread(mStreams);
	if (mAudioThread != null) {
	    mAudioThread.start();
	}
	// call C
	EWOL.onResume();
    }
	
    @Override protected void onPause()
    {
	super.onPause();
	mGLView.onPause();
	if (mAudioThread != null) {
	    // request audio stop
	    mStreams.AutoStop();
	    // wait the thread ended ...
	    try {
		mAudioThread.join();
	    } catch(InterruptedException e) { }
	}
	// call C
	EWOL.onPause();
    }
	
    @Override protected void onStop()
    {
	super.onStop();
	// call C
	EWOL.onStop();
    }
    @Override protected void onDestroy()
    {
	super.onDestroy();
	// call C
	EWOL.onDestroy();
	// Remove the java Virtual machine pointer form the C code
	Ewol.setJavaVirtualMachineStop();
    }
	
    @Override protected void finalize() throws Throwable
    {
	super.finalize();
	// cleanup your object here
    }
	
    public void onConfigurationChanged(Configuration newConfig)
    {
	super.onConfigurationChanged(newConfig);
    }
	
    @Override public void keyboardUpdate(boolean show)
    {
	final InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
	if(show) {
	    //EWOL.touchEvent();
	    imm.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
	} else {
	imm.toggleSoftInput(0 ,InputMethodManager.HIDE_IMPLICIT_ONLY + InputMethodManager.HIDE_NOT_ALWAYS);
	//imm.hideSoftInputFromWindow(view.getWindowToken(),0); 
	}
    }
	
    @Override public static void eventNotifier(String[] args)
    {
	// just for the test ...
	EWOL.touchEvent();
    }
	
    public void orientationUpdate(int screenMode)
    {
	if (screenMode == EWOL_ORIENTATION_LANDSCAPE) {
	    //Force landscape
	    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	} else if (screenMode == EWOL_ORIENTATION_PORTRAIT) {
	    //Force portrait
	    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	} else {
	    //Force auto Rotation
	    setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
	}
    }
}



