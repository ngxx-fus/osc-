
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

void sdlFontInit(){
    __entry("font_init()");
    if (TTF_Init() < 0) {
        __err("[font_init] TTF_Init failed: %s\n", TTF_GetError());
        return ;
    }
    font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        __err("[font_init] Error loading font: %s\n", TTF_GetError());
        return ;
    }
    __exit("font_init()");
}

void sdlFontExit(){
    __entry("simFontExit()");
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    TTF_Quit();
    __exit("simFontExit()");
}

void sdlInit(){
    __entry("screen_init()");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[screen_init] SDL_Init failed: %s\n", SDL_GetError());
        return;
    }
    gWindow = SDL_CreateWindow(
    WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        screenW, screenH, 
    SDL_WINDOW_SHOWN
    );
    gRenderer = SDL_CreateRenderer(
        gWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    gTexture = SDL_CreateTexture(
        gRenderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        screenW,
        screenH
    );
    if (!gTexture) {
        __log("Create gTexture failed: %s", SDL_GetError());
        exit(1);
    }
    __exit("screen_init()");
}

/// OSC INIT & EXIT ///////////////////////////////////////////////////////////////////////////////

void oscInit(){
    __entry("oscInit()");
    statusFlag setFlag (STARTUP);
    screenBuffer = (color_t *) malloc(sizeof(color_t) * screenH * screenW);
    sdlInit();
    sdlFontInit();
    screenFlag  setFlag (BUFFER_FLUSH);
    statusFlag setFlag (RUNNING);
    __exit("oscInit()");
}

void oscExit(){
    __entry("oscInit()");
    sdlFontExit();
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
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
