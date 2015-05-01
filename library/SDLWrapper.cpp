#include "SDLWrapper.h"
using namespace DFHack;
static DFLibrary *sdl_handle = NULL;
static DFLibrary *sdl_image_handle = NULL;
static DFLibrary *sdl_ttf_handle = NULL;

#if defined(_LINUX)
    const char *DFHack::SDLWrapper::sdl_path        = "libSDL-1.2.so.0";
    const char *DFHack::SDLWrapper::sdl_image_path  = "libSDL_image-1.2.so.0";
    const char *DFHack::SDLWrapper::sdl_ttf_path    = "libSDL_ttf-2.0.so.0";
#elif defined(_DARWIN)
    const char *DFHack::SDLWrapper::sdl_path        = "SDL.framework/SDL";
    const char *DFHack::SDLWrapper::sdl_image_path  = "SDL_image.framework/SDL_image";
    const char *DFHack::SDLWrapper::sdl_ttf_path    = "SDL_ttf.framework/SDL_ttf";
#else
    const char *DFHack::SDLWrapper::sdl_path        = "SDL.dll";
    const char *DFHack::SDLWrapper::sdl_image_path  = "SDL_image.dll";
    const char *DFHack::SDLWrapper::sdl_ttf_path    = "SDL_ttf.dll";
#endif

void load_lib (DFLibrary **dest, const char *path, const char *desc)
{
    *dest = OpenPlugin(path);
    if (!*dest)
    {
        fprintf(stderr, "Could not load %s library (%s)\n", desc, path);
    }
}

void load_sym (void **dest, DFLibrary **lib, const char *name)
{
    *dest = LookupPlugin(*lib, name);
    if (!*dest)
    {
        fprintf(stderr, "Could not load symbol: %s\n", name);
    }
}

namespace DFHack { namespace SDLWrapper {
    bool loaded() { return sdl_loaded() && sdl_image_loaded() && sdl_ttf_loaded(); }
    bool sdl_loaded() { return sdl_handle; }
    bool sdl_image_loaded() { return sdl_image_handle; }
    bool sdl_ttf_loaded() { return sdl_ttf_handle; }
    DFLibrary *get_sdl() { return sdl_handle; };
    DFLibrary *get_sdl_image() { return sdl_image_handle; };
    DFLibrary *get_sdl_ttf() { return sdl_ttf_handle; };

    bool init()
    {
#define sdl_load_lib(lib, name) load_lib(&lib##_handle, lib##_path, name);
        sdl_load_lib(sdl, "SDL");
        sdl_load_lib(sdl_image, "SDL_image");
        sdl_load_lib(sdl_ttf, "SDL_ttf");
#undef sdl_load_lib
#define SDL_WRAP_FUNC(lib, rtype, name, args) load_sym((void**)&DFHack::SDLWrapper::name, &lib##_handle, #name)
#include "SDLWrapperStubs.h"
#undef SDL_WRAP_FUNC
        return loaded();
    }

#define SDL_WRAP_FUNC(lib, rtype, name, args) rtype (*name) args = NULL;
#include "SDLWrapperStubs.h"
#undef SDL_WRAP_FUNC
}}
