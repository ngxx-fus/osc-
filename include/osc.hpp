
#include <iostream>
using namespace std;

/// HEADERS ///////////////////////////////////////////////////////////////////////////////////////

#include "global.h"

/// VARS //////////////////////////////////////////////////////////////////////////////////////////

#define WINDOW_NAME "ngxxfus' osc"

extern volatile flag_t statusFlag;
extern volatile flag_t screenFlag;

enum ENUM_STATUS_FLAG_BITORDER{
    STARTUP = 0,
    RUNNING = 1,
    STOPPED = 2,
};

enum ENUM_SCREEN_FLAG_BITORDER{
    BUFFER_FLUSH = 0,
};

/// INIT & EXIT ///////////////////////////////////////////////////////////////////////////////////


/// OSC INIT & EXIT ///////////////////////////////////////////////////////////////////////////////

void oscInit(){
    __entry("oscInit()");
    statusFlag setFlag (STARTUP);
    screenBuffer = (color_t *) malloc(sizeof(color_t) * screenH * screenW);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        __err("[oscInit] SDL_Init failed: %s\n", SDL_GetError());
        return;
    }
    if (TTF_Init() < 0) {
        __err("[oscInit] TTF_Init failed: %s\n", TTF_GetError());
        return ;
    }
    createWindowContext(
        &mainWindow, screenW, screenH, "ngxxfus' osc", 
        FONT_PATH, FONT_SIZE
    );
    screenFlag  setFlag (BUFFER_FLUSH);
    statusFlag setFlag (RUNNING);
    __exit("oscInit()");
}

void oscExit(){
    __entry("oscInit()");
    destroyWindowContext(&mainWindow);
    SDL_Quit();
    
    free(screenBuffer);
    __exit("oscInit()");
}

/// THREADS ///////////////////////////////////////////////////////////////////////////////////////

int inputService(void * pv){
    __entry("inputService()");
    SDL_Event e;
    while(statusFlag hasFlag (RUNNING)){
        while (SDL_PollEvent(&e)) {
            switch (e.type)
            {
                case SDL_QUIT:
                    __log("Event <SDL_QUIT> occured!");
                    statusFlag = statusFlag & fInvMask(RUNNING) | fMask(STOPPED);
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) {
                        __log("Event: <SDLK_q> is pressed!");
                        statusFlag = statusFlag & fInvMask(RUNNING) | fMask(STOPPED);
                    }else 
                    if(e.key.keysym.sym == SDLK_c){
                        __log("Event <SDL_KEYDOWN | SDLK_c> occured!");
                    }else 
                    if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                    }
                    break;
                case SDL_KEYUP:
                    if(SDLK_0 <= e.key.keysym.sym && e.key.keysym.sym <= SDLK_9){
                        uint8_t i = e.key.keysym.sym - SDLK_0;
                    }else{
                        if(e.key.keysym.sym == SDLK_c){
                        }
                    }
                    break;
            }
        }
    }
    __exit("inputService()");
    return 0;
}
