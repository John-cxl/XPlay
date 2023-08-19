//
// Created by John on 2023-08-08.
//

#include "IPlayerPorxy.h"
#include "FFPlayerBuilder.h"
void IPlayerPorxy::Init(void* vm) {
    mux.lock();
    if(vm)
        FFPlayerBuilder::InitHard(vm);
    if(!player)
        player = FFPlayerBuilder::Get()->BuilderPlayer();
    mux.unlock();
}
void  IPlayerPorxy::Close()
{
    mux.lock();
    if(player)
        player->Close();
    mux.unlock();
}


double IPlayerPorxy::PlayPos()
{
    double pos = 0.0;
    mux.lock();
    if(player)
        pos = player->PlayPos();
    mux.unlock();

    return pos;
}
bool IPlayerPorxy::Seek(double pos)
{
    bool b = false;
    mux.lock();
    if(player)
        b = player->Seek(pos);
    mux.unlock();
    return b;
}

bool IPlayerPorxy::Open(const char *filePath) {
    bool b = false;
    mux.lock();
    if(player)
    {
        player->isHardDecode = isHardDecode;
        b = player->Open(filePath);
    }

    mux.unlock();
    return b;
}

bool IPlayerPorxy::Start() {
    bool b = false;
    mux.lock();
    if(player)
        b = player->Start();
    mux.unlock();
    return b;
}

bool IPlayerPorxy::InitView(void *pWin) {
    bool b = false;
    mux.lock();
    if(player)
        b =  player->InitView(pWin);
    mux.unlock();
    return b;
}


