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

bool IPlayerPorxy::Open(const char *filePath) {
    bool b = false;
    mux.lock();
    if(player)
       b = player->Open(filePath);
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


