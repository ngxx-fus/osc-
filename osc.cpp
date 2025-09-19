#include "include/osc.hpp"

/// GLOBAL VARS ///////////////////////////////////////////////////////////////////////////////////

const xy_t       screenW = 640;
const xy_t       screenH = 480;

SDL_Window       *gWindow;                   /// Main application window
SDL_Renderer     *gRenderer;                 /// Main SDL renderer used for drawing
SDL_Texture      *gTexture;                  /// General-purpose texture (frame buffer or image)
TTF_Font         *font;                      /// Global font resource
pthread_mutex_t  sdlMutex = PTHREAD_MUTEX_INITIALIZER;                   /// Mutex lock for SDL operations (thread-safety)
pthread_mutex_t  scrBufMutex = PTHREAD_MUTEX_INITIALIZER;                /// Mutex (mutual exclusion) lock for screen buffer
color_t *        screenBuffer;

volatile flag_t  statusFlag = 0;
volatile flag_t  screenFlag = 0;



/// MAIN FUNCTION /////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** args) {
    __entry("main()");
    /// SETUP EXIT CALLBACK ///////////////////////////////////////////////////////////////////////
    atexit(oscExit);
    /// CALL INIT /////////////////////////////////////////////////////////////////////////////////
    oscInit();
    /// NEW THREADS ///////////////////////////////////////////////////////////////////////////////
    
    __log("[main] [+] InputThread");
    SDL_Thread *thread0 = SDL_CreateThread(inputService, "inputService", NULL);
    if (!thread0) {
        __log("[main] Create InputThread failed: %s", SDL_GetError());
        exit(-1);
    }

    /// MAIN THREAD ///////////////////////////////////////////////////////////////////////////////
    __log("[main] Entry mainSloop");
    while (statusFlag hasFlag (RUNNING)) {
        if(screenFlag  hasFlag (BUFFER_FLUSH)){
            // __sim_log("[eventProcessLoop] Has a render request!");
            screenFlag  clrFlag (BUFFER_FLUSH);
            __entryCriticalSection(&sdlMutex);
            SDL_UpdateTexture(gTexture, NULL, screenBuffer, screenW * sizeof(color_t));
            // Clear background với màu đen, alpha=255 (opaque)
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
            SDL_RenderClear(gRenderer);

            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
            SDL_RenderPresent(gRenderer);
            __exitCriticalSection(&sdlMutex);
        }
        #if defined(RENDER_FPS)
            // __log("Delay for %dms!", 1000/RENDER_FPS);
            SDL_Delay(1000/RENDER_FPS);
        #else 
            SDL_Delay(50);
        #endif
    }
    __log("[main] Exit mainSloop");
    
    __exit("main()");
    return 0;
}