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

public class XPlay extends SurfaceView{

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.d("chx", "XPlay00000000000000");
    }

    public XPlay(Context context)
    {
        super(context);
    }
//    @Override
//    public void surfaceCreated(SurfaceHolder holder)
//    {
//        Log.d("chx", "surfaceCreated1111111111111111111");
//        //initView(holder.getSurface());
//    }
//
//    @Override
//    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
//    {
//        Log.d("chx", "surfaceChanged22222222222222222222");
//    }
//
//    @Override
//    public void surfaceDestroyed(SurfaceHolder holder)
//    {
//        Log.d("chx", "surfaceDestroyed333333333333333333333333");
//    }

    public native void initView(Object view);
}
