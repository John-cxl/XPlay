package com.example.xplay;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.support.annotation.IntRange;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlay extends SurfaceView /*implements SurfaceHolder.Callback, GLSurfaceView.Renderer*/{

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.d("chx", "XPlay00000000000000");
        //setRenderer(this);
    }

    public XPlay(Context context)
    {
        super(context);
    }
    public native void initView(Object view);
//    @Override
//    public void surfaceCreated(SurfaceHolder holder)
//    {
//        Log.d("chx", "surfaceCreated1111111111111111111");
//        initView(holder.getSurface());
//        //安卓8.0需要设置  生命周期的问题
//    }
//
//    @Override
//    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
//    {
//        //Log.d("chx", "surfaceChanged22222222222222222222");
//    }
//
//    @Override
//    public void surfaceDestroyed(SurfaceHolder holder)
//    {
//        //Log.d("chx", "surfaceDestroyed333333333333333333333333");
//    }
//

//
//    @Override
//    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
//
//    }
//
//    @Override
//    public void onSurfaceChanged(GL10 gl, int width, int height) {
//
//    }
//
//    @Override
//    public void onDrawFrame(GL10 gl) {
//
//    }
}
