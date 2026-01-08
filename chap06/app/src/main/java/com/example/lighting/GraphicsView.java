package com.example.lighting;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;


public class GraphicsView extends GLSurfaceView 
{
	/*[Private member variables]*/
	protected int redSize = 8;
	protected int greenSize = 8;
	protected int blueSize = 8;
	protected int alphaSize = 8;

	// 16 bits for depth which is needed
	// To make object displayed in the right order
	protected int depthSize = 16;

	// Enabling anti-aliasing (x4)
	protected int sampleSize = 4;

	// Stencil buffer
	protected int stencilSize = 0;
	protected int[] value = new int[1];

	/*[constructor]*/
	public GraphicsView(Context ctx)
	{
	 	super(ctx);
        init();
	}

	// Constructeur XML (OBLIGATOIRE)
    public GraphicsView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

	private void init()
	{
		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser(new ConfigChooser());
		setRenderer(new Renderer());
		setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}

	/*[ContextFactory]*/
	private class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			final int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

			int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE};

			EGLContext ctx = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);

			return ctx;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext ctx)
		{
			egl.eglDestroyContext(display, ctx);
		}
	}
	/*[end ContextFactory]*/

	/*[ConfigChooser]*/
	protected class ConfigChooser implements GLSurfaceView.EGLConfigChooser
	{
		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
		{
			final int EGL_OPENGL_ES2_BIT = 4;

			int[] configAttributes = 
			{
				EGL10.EGL_RED_SIZE, redSize,
                EGL10.EGL_GREEN_SIZE, greenSize,
                EGL10.EGL_BLUE_SIZE, blueSize,
                EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,		// OpenGL ES 2.0.
                EGL10.EGL_SAMPLES, sampleSize,
                EGL10.EGL_DEPTH_SIZE, depthSize,
                EGL10.EGL_STENCIL_SIZE, stencilSize,
                EGL10.EGL_NONE
			};

			int[] num_config = new int[1];
			egl.eglChooseConfig(display, configAttributes, null, 0, num_config);

			int numConfigs = num_config[0];

			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, configAttributes, configs, numConfigs, num_config);

			return selectConfig(egl, display, configs);
		}

		/* [selectConfig] */
		public EGLConfig selectConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			for (EGLConfig config : configs)
			{
				int d = getConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = getConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int r = getConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE,0);
                int g = getConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
                int b = getConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
                int a = getConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

                if(r == redSize && g == greenSize && b == blueSize && a == alphaSize
            	&& d >= depthSize && s >= stencilSize) {return config;}
			}
			return null;
		}

		private int getConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config,
				int attribute, int defaultValue)
		{
			if(egl.eglGetConfigAttrib(display, config, attribute, value))
				return value[0];

			return defaultValue;
		}

	}/*[end ConfigChooser]*/

	/* [Renderer] */
	private static class Renderer implements GLSurfaceView.Renderer
	{
		public void onDrawFrame(GL10 gl)
		{
			NativeLibrary.render();
		}
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			NativeLibrary.loadResources();
		}
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			NativeLibrary.initGraphics(width, height);
			NativeLibrary.update(width, height);
		}
	}
}