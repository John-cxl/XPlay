package com.example.xplay;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

public class open_source extends AppCompatActivity implements View.OnClickListener
{
    Button m_btnFile;
    Button m_btnRtmp;
    EditText m_etFile;
    EditText m_etRtmp;
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        supportRequestWindowFeature(Window.FEATURE_NO_TITLE);//去掉 标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//全屏 隐藏状态
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); //屏幕为 横屏


        setContentView(R.layout.open_source_layout);
        m_btnFile = findViewById(R.id.btn_file);
        m_btnRtmp = findViewById(R.id.btn_rtmp);
        m_btnFile.setOnClickListener(this);
        m_btnRtmp.setOnClickListener(this);
        m_etFile = findViewById(R.id.et_file);
        m_etRtmp = findViewById(R.id.et_rtmp);
    }

    @Override
    public void onClick(View v) {
        switch(v.getId())
        {
            case R.id.btn_file:
                Open(m_etFile.getText().toString());
                finish();
                break;
            case R.id.btn_rtmp:
                Open(m_etRtmp.getText().toString());
                finish();
                break;
        }
    }

    public native void Open(String url);
}
