package com.example.xplay;

import android.content.Intent;
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
import android.widget.SeekBar;
import android.widget.TextView;



public class MainActivity extends AppCompatActivity implements View.OnClickListener , SurfaceHolder.Callback, Runnable
{

    // Used to load the 'xplay' library on application startup.
    static {
        System.loadLibrary("xplay");
    }
    SurfaceHolder surfaceHolder;
    private XPlay m_xplay;
    private Button m_btnOpen;
    private SeekBar m_skProcess;
    private Thread m_thread;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);//去掉 标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//全屏 隐藏状态
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); //屏幕为 横屏

        setContentView(R.layout.activity_main);
        m_xplay =  findViewById(R.id.id_xplay);
        m_btnOpen = findViewById(R.id.btn_open);
        m_skProcess = findViewById(R.id.sk_process);
        inicSurfaceHolder();
        m_btnOpen.setOnClickListener(this);
        m_skProcess.setMax(1000);

        //启动进程来显示进度条状态
        m_thread = new Thread(this);
        m_thread.start();
        m_skProcess.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Seek((double)seekBar.getProgress() / (double)seekBar.getMax());
            }
        });
        stringFromJNI();
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
    public void onClick(View v) {
        switch(v.getId())
        {
            case R.id.btn_open:
            {
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, open_source.class);
                startActivity(intent);
            }
            break;
        }
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        Log.d("chx","surfaceCreated");
        m_xplay.initView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void run() {
        for(;;)
        {

            m_skProcess.setProgress((int)(PlayPos() * 1000));
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public  native double PlayPos();
    public native void Seek(double pos);
}