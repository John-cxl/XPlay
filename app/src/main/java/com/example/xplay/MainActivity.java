package com.example.xplay;

import android.content.pm.ActivityInfo;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;



public class MainActivity extends AppCompatActivity  /*implements SurfaceHolder.Callback*/{

    // Used to load the 'xplay' library on application startup.
    static {
        System.loadLibrary("xplay");
    }
    SurfaceHolder surfaceHolder;
    private XPlay m_xplay;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);//去掉 标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//全屏 隐藏状态
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); //屏幕为 横屏


        setContentView(R.layout.activity_main);
        m_xplay =  findViewById(R.id.id_xplay);
        //inicSurfaceHolder();

        stringFromJNI();

    }

    /**
     * A native method that is implemented by the 'xplay' native library,
     * which is packaged with this application.
     */

//    private void inicSurfaceHolder(){
//        surfaceHolder= m_xplay.getHolder();
//
//        //surfaceHolder.addCallback(MainActivity.this);
//        m_xplay.setVisibility(View.VISIBLE);
//        Log.d("chx","inicCamera");
//    }

    public native String stringFromJNI();

//    @Override
//    public void surfaceCreated(@NonNull SurfaceHolder holder) {
//        m_xplay.initView(holder.getSurface());
//    }
//
//    @Override
//    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
//
//    }
//
//    @Override
//    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
//
//    }
}