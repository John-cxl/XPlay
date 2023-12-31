//
// Created by John on 2023-08-08.
//

#ifndef XPLAY_IPLAYERPORXY_H
#define XPLAY_IPLAYERPORXY_H

#include <mutex>
#include "IPlayer.h"

class IPlayerPorxy: public IPlayer {
public:
    static IPlayerPorxy * Get()
    {
        static IPlayerPorxy porxy;
        return &porxy;
    }

    void Init(void* vm);
    virtual bool Open(const char* filePath);
    virtual void Close();

    virtual void SetPause(bool isP);
    virtual bool IsPause();
    virtual bool Start();
    virtual bool InitView(void* pWin);
    //获取当前的播放进度  0.0 --- 1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
protected:
    IPlayerPorxy(){}
    IPlayer * player = 0;
    std::mutex mux;
};


#endif //XPLAY_IPLAYERPORXY_H
