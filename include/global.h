#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// #define LOG_HEADER_INCLUDE
#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: global.h")
#endif

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

/// APP'S HEADER //////////////////////////////////////////////////////////////////////////////////
#include "color.h"
#include "helper.h"
#include "../lib/log/log.h"
#include "../lib/windowContext/windowContext.h"

/// GLOBL VARS ///////////////////////////////////////////////////////////////////////////////////
#define FONT_PATH       "/usr/share/fonts/TTF/DejaVuSans.ttf"
#define FONT_SIZE       12

typedef int (*pThreadFunc_t)(void*);
typedef int32_t             xy_t;                       /// Size type, x for horizontal (0...H), y for vertical (0...W)
typedef int32_t             color_t;                    /// Color type (RGBA)
typedef uint64_t            flag_t;                     /// Flag type, 64-bit

#define fMask(f)            __mask64(f)
#define fInvMask(f)         __inv_mask64(f)
#define hasFlag(f)          & fMask(f)
#define setFlag(f)          |= fMask(f)
#define clrFlag(f)          &= fInvMask(f)

extern windowContext_t *    mainWindow;
extern pthread_mutex_t      sdlMutex;                   /// Mutex lock for SDL operations (thread-safety)
extern pthread_mutex_t      scrBufMutex;                /// Mutex lock for screen buffer
extern const xy_t           screenW;                    /// Y
extern const xy_t           screenH;                    /// X
extern color_t *            screenBuffer;

#define bufferPixel(x, y)   screenBuffer[(x) * screenW + (y)]

///////////////////////////////////////////////////////////////////////////////////////////////////


#endif