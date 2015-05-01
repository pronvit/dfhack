#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "Export.h"
namespace DFHack {
    struct DFLibrary;
    DFHACK_EXPORT DFLibrary *OpenPlugin (const char*);
    DFHACK_EXPORT void *LookupPlugin (DFLibrary*, const char*);
    DFHACK_EXPORT void ClosePlugin (DFLibrary*);

    namespace SDLWrapper {
#ifndef SDL_EVENTMASK
#define SDL_EVENTMASK(X)    (1<<(X))
#endif
        typedef int8_t Sint8;
        typedef int16_t Sint16;
        typedef int32_t Sint32;
        typedef int64_t Sint64;
        typedef uint8_t Uint8;
        typedef uint16_t Uint16;
        typedef uint32_t Uint32;
        typedef uint64_t Uint64;
        typedef Uint32 (*SDL_TimerCallback)(Uint32 interval);
        typedef Uint32 (*SDL_NewTimerCallback)(Uint32 interval, void *param);

        typedef struct SDL_Color {
                Uint8 r;
                Uint8 g;
                Uint8 b;
                Uint8 unused;
        } SDL_Color;
        #ifndef SDL_Colour
        #define SDL_Colour SDL_Color
        #endif
        typedef struct SDL_version {
                Uint8 major;
                Uint8 minor;
                Uint8 patch;
        } SDL_version;
        typedef struct SDL_Palette {
                int       ncolors;
                SDL_Color *colors;
        } SDL_Palette;
        /* Everything in the pixel format structure is read-only */
        typedef struct SDL_PixelFormat {
                SDL_Palette *palette;
                Uint8  BitsPerPixel;
                Uint8  BytesPerPixel;
                Uint8  Rloss;
                Uint8  Gloss;
                Uint8  Bloss;
                Uint8  Aloss;
                Uint8  Rshift;
                Uint8  Gshift;
                Uint8  Bshift;
                Uint8  Ashift;
                Uint32 Rmask;
                Uint32 Gmask;
                Uint32 Bmask;
                Uint32 Amask;

                /* RGB color key information */
                Uint32 colorkey;
                /* Alpha value information (per-surface alpha) */
                Uint8  alpha;
        } SDL_PixelFormat;
        typedef struct SDL_VideoInfo {
                Uint32 hw_available :1; /* Flag: Can you create hardware surfaces? */
                Uint32 wm_available :1; /* Flag: Can you talk to a window manager? */
                Uint32 UnusedBits1  :6;
                Uint32 UnusedBits2  :1;
                Uint32 blit_hw      :1; /* Flag: Accelerated blits HW --> HW */
                Uint32 blit_hw_CC   :1; /* Flag: Accelerated blits with Colorkey */
                Uint32 blit_hw_A    :1; /* Flag: Accelerated blits with Alpha */
                Uint32 blit_sw      :1; /* Flag: Accelerated blits SW --> HW */
                Uint32 blit_sw_CC   :1; /* Flag: Accelerated blits with Colorkey */
                Uint32 blit_sw_A    :1; /* Flag: Accelerated blits with Alpha */
                Uint32 blit_fill    :1; /* Flag: Accelerated color fill */
                Uint32 UnusedBits3  :16;
                Uint32 video_mem;       /* The total amount of video memory (in K) */
                SDL_PixelFormat *vfmt;  /* Value: The format of the video surface */
                int    current_w;       /* Value: The current video mode width */
                int    current_h;       /* Value: The current video mode height */
        } SDL_VideoInfo;

        extern const char *sdl_path;
        extern const char *sdl_image_path;
        extern const char *sdl_ttf_path;
        bool init();
        bool loaded();
        bool sdl_loaded();
        bool sdl_image_loaded();
        bool sdl_ttf_loaded();
        DFLibrary *get_sdl();
        DFLibrary *get_sdl_image();
        DFLibrary *get_sdl_ttf();
#define SDL_WRAP_FUNC(handle, rtype, name, args) DFHACK_EXPORT extern rtype (*name) args;
#define SDL_WRAP_DEFINE_STRUCTS
#include "SDLWrapperStubs.h"
#undef SDL_WRAP_DEFINE_STRUCTS
#undef SDL_WRAP_FUNC
    }
}
