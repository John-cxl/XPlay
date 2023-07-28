package com.example.xplay;

import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;



public class MainActivity extends AppCompatActivity  implements SurfaceHolder.Callback{

    // Used to load the 'xplay' library on application startup.
    static {
        System.loadLibrary("xplay");
    }
    SurfaceHolder surfaceHolder;
    private XPlay m_xplay;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        binding = ActivityMainBinding.inflate(getLayoutInflater());
//        setContentView(binding.getRoot());
        setContentView(R.layout.activity_main);
        m_xplay =  findViewById(R.id.id_xplay);
        inicSurfaceHolder();
       // m_xplay.setZOrderOnTop(true);
//        m_xplay = new XPlay(this);
//        setContentView(m_xplay);
        stringFromJNI();
        //m_xplay.setEGLContextClientVersion(2);
        // Example of a call to a native method
        //TextView tv = findViewById(R.id.sample_text);
//        m_xplay = findViewById(R.id.id_xplay);
        //tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'xplay' native library,
     * which is packaged with this application.
     */

    private void inicSurfaceHolder(){
        surfaceHolder= m_xplay.getHolder();

        surfaceHolder.addCallback(MainActivity.this);
        m_xplay.setVisibility(View.VISIBLE);
        Log.d("chx","inicCamera");
    }

    public native String stringFromJNI();

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        m_xplay.initView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }
}