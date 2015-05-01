// Generated - do not edit
#ifdef SDL_WRAP_DEFINE_STRUCTS
struct SDL_AudioSpec;
struct SDL_RWops;
struct SDL_AudioCVT;
struct SDL_CD;
struct SDL_Event;
struct SDL_EventFilter;
struct SDL_Joystick;
struct QDGlobals;
struct SDL_Cursor;
struct SDL_mutex;
struct SDL_sem;
struct SDL_cond;
struct SDL_iconv_t;
struct SDL_SysWMinfo;
struct SDL_Thread;
struct SDL_TimerID;
struct SDL_PixelFormat;
struct SDL_Surface;
struct SDL_Rect;
struct SDL_Color;
struct SDL_Overlay;
struct TTF_Font;
typedef enum {
    SDL_AUDIO_STOPPED = 0,
    SDL_AUDIO_PLAYING,
    SDL_AUDIO_PAUSED
} SDL_audiostatus;
typedef enum {
    CD_TRAYEMPTY,
    CD_STOPPED,
    CD_PLAYING,
    CD_PAUSED,
    CD_ERROR = -1
} CDstatus;
typedef enum {
    SDL_ENOMEM,
    SDL_EFREAD,
    SDL_EFWRITE,
    SDL_EFSEEK,
    SDL_UNSUPPORTED,
    SDL_LASTERROR
} SDL_errorcode;
typedef enum {
       SDL_NOEVENT = 0,         /* Unused (do not remove) */
       SDL_ACTIVEEVENT,         /* Application loses/gains visibility */
       SDL_KEYDOWN,         /* Keys pressed */
       SDL_KEYUP,           /* Keys released */
       SDL_MOUSEMOTION,         /* Mouse moved */
       SDL_MOUSEBUTTONDOWN,     /* Mouse button pressed */
       SDL_MOUSEBUTTONUP,       /* Mouse button released */
       SDL_JOYAXISMOTION,       /* Joystick axis motion */
       SDL_JOYBALLMOTION,       /* Joystick trackball motion */
       SDL_JOYHATMOTION,        /* Joystick hat position change */
       SDL_JOYBUTTONDOWN,       /* Joystick button pressed */
       SDL_JOYBUTTONUP,         /* Joystick button released */
       SDL_QUIT,            /* User-requested quit */
       SDL_SYSWMEVENT,          /* System specific event */
       SDL_EVENT_RESERVEDA,     /* Reserved for future use.. */
       SDL_EVENT_RESERVEDB,     /* Reserved for future use.. */
       SDL_VIDEORESIZE,         /* User resized video mode */
       SDL_VIDEOEXPOSE,         /* Screen needs to be redrawn */
       SDL_EVENT_RESERVED2,     /* Reserved for future use.. */
       SDL_EVENT_RESERVED3,     /* Reserved for future use.. */
       SDL_EVENT_RESERVED4,     /* Reserved for future use.. */
       SDL_EVENT_RESERVED5,     /* Reserved for future use.. */
       SDL_EVENT_RESERVED6,     /* Reserved for future use.. */
       SDL_EVENT_RESERVED7,     /* Reserved for future use.. */
       /* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
       SDL_USEREVENT = 24,
       /* This last event is only for bounding internal arrays
      It is the number of bits in the event mask datatype -- Uint32
        */
       SDL_NUMEVENTS = 32
} SDL_EventType;
typedef enum {
    SDL_ACTIVEEVENTMASK = SDL_EVENTMASK(SDL_ACTIVEEVENT),
    SDL_KEYDOWNMASK     = SDL_EVENTMASK(SDL_KEYDOWN),
    SDL_KEYUPMASK       = SDL_EVENTMASK(SDL_KEYUP),
    SDL_KEYEVENTMASK    = SDL_EVENTMASK(SDL_KEYDOWN)|
                              SDL_EVENTMASK(SDL_KEYUP),
    SDL_MOUSEMOTIONMASK = SDL_EVENTMASK(SDL_MOUSEMOTION),
    SDL_MOUSEBUTTONDOWNMASK = SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN),
    SDL_MOUSEBUTTONUPMASK   = SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
    SDL_MOUSEEVENTMASK  = SDL_EVENTMASK(SDL_MOUSEMOTION)|
                              SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)|
                              SDL_EVENTMASK(SDL_MOUSEBUTTONUP),
    SDL_JOYAXISMOTIONMASK   = SDL_EVENTMASK(SDL_JOYAXISMOTION),
    SDL_JOYBALLMOTIONMASK   = SDL_EVENTMASK(SDL_JOYBALLMOTION),
    SDL_JOYHATMOTIONMASK    = SDL_EVENTMASK(SDL_JOYHATMOTION),
    SDL_JOYBUTTONDOWNMASK   = SDL_EVENTMASK(SDL_JOYBUTTONDOWN),
    SDL_JOYBUTTONUPMASK = SDL_EVENTMASK(SDL_JOYBUTTONUP),
    SDL_JOYEVENTMASK    = SDL_EVENTMASK(SDL_JOYAXISMOTION)|
                              SDL_EVENTMASK(SDL_JOYBALLMOTION)|
                              SDL_EVENTMASK(SDL_JOYHATMOTION)|
                              SDL_EVENTMASK(SDL_JOYBUTTONDOWN)|
                              SDL_EVENTMASK(SDL_JOYBUTTONUP),
    SDL_VIDEORESIZEMASK = SDL_EVENTMASK(SDL_VIDEORESIZE),
    SDL_VIDEOEXPOSEMASK = SDL_EVENTMASK(SDL_VIDEOEXPOSE),
    SDL_QUITMASK        = SDL_EVENTMASK(SDL_QUIT),
    SDL_SYSWMEVENTMASK  = SDL_EVENTMASK(SDL_SYSWMEVENT)
} SDL_EventMask ;
typedef enum {
    SDL_ADDEVENT,
    SDL_PEEKEVENT,
    SDL_GETEVENT
} SDL_eventaction;
typedef enum {
    /* The keyboard syms have been cleverly chosen to map to ASCII */
    SDLK_UNKNOWN        = 0,
    SDLK_FIRST      = 0,
    SDLK_BACKSPACE      = 8,
    SDLK_TAB        = 9,
    SDLK_CLEAR      = 12,
    SDLK_RETURN     = 13,
    SDLK_PAUSE      = 19,
    SDLK_ESCAPE     = 27,
    SDLK_SPACE      = 32,
    SDLK_EXCLAIM        = 33,
    SDLK_QUOTEDBL       = 34,
    SDLK_HASH       = 35,
    SDLK_DOLLAR     = 36,
    SDLK_AMPERSAND      = 38,
    SDLK_QUOTE      = 39,
    SDLK_LEFTPAREN      = 40,
    SDLK_RIGHTPAREN     = 41,
    SDLK_ASTERISK       = 42,
    SDLK_PLUS       = 43,
    SDLK_COMMA      = 44,
    SDLK_MINUS      = 45,
    SDLK_PERIOD     = 46,
    SDLK_SLASH      = 47,
    SDLK_0          = 48,
    SDLK_1          = 49,
    SDLK_2          = 50,
    SDLK_3          = 51,
    SDLK_4          = 52,
    SDLK_5          = 53,
    SDLK_6          = 54,
    SDLK_7          = 55,
    SDLK_8          = 56,
    SDLK_9          = 57,
    SDLK_COLON      = 58,
    SDLK_SEMICOLON      = 59,
    SDLK_LESS       = 60,
    SDLK_EQUALS     = 61,
    SDLK_GREATER        = 62,
    SDLK_QUESTION       = 63,
    SDLK_AT         = 64,
    /*
       Skip uppercase letters
     */
    SDLK_LEFTBRACKET    = 91,
    SDLK_BACKSLASH      = 92,
    SDLK_RIGHTBRACKET   = 93,
    SDLK_CARET      = 94,
    SDLK_UNDERSCORE     = 95,
    SDLK_BACKQUOTE      = 96,
    SDLK_a          = 97,
    SDLK_b          = 98,
    SDLK_c          = 99,
    SDLK_d          = 100,
    SDLK_e          = 101,
    SDLK_f          = 102,
    SDLK_g          = 103,
    SDLK_h          = 104,
    SDLK_i          = 105,
    SDLK_j          = 106,
    SDLK_k          = 107,
    SDLK_l          = 108,
    SDLK_m          = 109,
    SDLK_n          = 110,
    SDLK_o          = 111,
    SDLK_p          = 112,
    SDLK_q          = 113,
    SDLK_r          = 114,
    SDLK_s          = 115,
    SDLK_t          = 116,
    SDLK_u          = 117,
    SDLK_v          = 118,
    SDLK_w          = 119,
    SDLK_x          = 120,
    SDLK_y          = 121,
    SDLK_z          = 122,
    SDLK_DELETE     = 127,
    /* End of ASCII mapped keysyms */

    /* International keyboard syms */
    SDLK_WORLD_0        = 160,      /* 0xA0 */
    SDLK_WORLD_1        = 161,
    SDLK_WORLD_2        = 162,
    SDLK_WORLD_3        = 163,
    SDLK_WORLD_4        = 164,
    SDLK_WORLD_5        = 165,
    SDLK_WORLD_6        = 166,
    SDLK_WORLD_7        = 167,
    SDLK_WORLD_8        = 168,
    SDLK_WORLD_9        = 169,
    SDLK_WORLD_10       = 170,
    SDLK_WORLD_11       = 171,
    SDLK_WORLD_12       = 172,
    SDLK_WORLD_13       = 173,
    SDLK_WORLD_14       = 174,
    SDLK_WORLD_15       = 175,
    SDLK_WORLD_16       = 176,
    SDLK_WORLD_17       = 177,
    SDLK_WORLD_18       = 178,
    SDLK_WORLD_19       = 179,
    SDLK_WORLD_20       = 180,
    SDLK_WORLD_21       = 181,
    SDLK_WORLD_22       = 182,
    SDLK_WORLD_23       = 183,
    SDLK_WORLD_24       = 184,
    SDLK_WORLD_25       = 185,
    SDLK_WORLD_26       = 186,
    SDLK_WORLD_27       = 187,
    SDLK_WORLD_28       = 188,
    SDLK_WORLD_29       = 189,
    SDLK_WORLD_30       = 190,
    SDLK_WORLD_31       = 191,
    SDLK_WORLD_32       = 192,
    SDLK_WORLD_33       = 193,
    SDLK_WORLD_34       = 194,
    SDLK_WORLD_35       = 195,
    SDLK_WORLD_36       = 196,
    SDLK_WORLD_37       = 197,
    SDLK_WORLD_38       = 198,
    SDLK_WORLD_39       = 199,
    SDLK_WORLD_40       = 200,
    SDLK_WORLD_41       = 201,
    SDLK_WORLD_42       = 202,
    SDLK_WORLD_43       = 203,
    SDLK_WORLD_44       = 204,
    SDLK_WORLD_45       = 205,
    SDLK_WORLD_46       = 206,
    SDLK_WORLD_47       = 207,
    SDLK_WORLD_48       = 208,
    SDLK_WORLD_49       = 209,
    SDLK_WORLD_50       = 210,
    SDLK_WORLD_51       = 211,
    SDLK_WORLD_52       = 212,
    SDLK_WORLD_53       = 213,
    SDLK_WORLD_54       = 214,
    SDLK_WORLD_55       = 215,
    SDLK_WORLD_56       = 216,
    SDLK_WORLD_57       = 217,
    SDLK_WORLD_58       = 218,
    SDLK_WORLD_59       = 219,
    SDLK_WORLD_60       = 220,
    SDLK_WORLD_61       = 221,
    SDLK_WORLD_62       = 222,
    SDLK_WORLD_63       = 223,
    SDLK_WORLD_64       = 224,
    SDLK_WORLD_65       = 225,
    SDLK_WORLD_66       = 226,
    SDLK_WORLD_67       = 227,
    SDLK_WORLD_68       = 228,
    SDLK_WORLD_69       = 229,
    SDLK_WORLD_70       = 230,
    SDLK_WORLD_71       = 231,
    SDLK_WORLD_72       = 232,
    SDLK_WORLD_73       = 233,
    SDLK_WORLD_74       = 234,
    SDLK_WORLD_75       = 235,
    SDLK_WORLD_76       = 236,
    SDLK_WORLD_77       = 237,
    SDLK_WORLD_78       = 238,
    SDLK_WORLD_79       = 239,
    SDLK_WORLD_80       = 240,
    SDLK_WORLD_81       = 241,
    SDLK_WORLD_82       = 242,
    SDLK_WORLD_83       = 243,
    SDLK_WORLD_84       = 244,
    SDLK_WORLD_85       = 245,
    SDLK_WORLD_86       = 246,
    SDLK_WORLD_87       = 247,
    SDLK_WORLD_88       = 248,
    SDLK_WORLD_89       = 249,
    SDLK_WORLD_90       = 250,
    SDLK_WORLD_91       = 251,
    SDLK_WORLD_92       = 252,
    SDLK_WORLD_93       = 253,
    SDLK_WORLD_94       = 254,
    SDLK_WORLD_95       = 255,      /* 0xFF */

    /* Numeric keypad */
    SDLK_KP0        = 256,
    SDLK_KP1        = 257,
    SDLK_KP2        = 258,
    SDLK_KP3        = 259,
    SDLK_KP4        = 260,
    SDLK_KP5        = 261,
    SDLK_KP6        = 262,
    SDLK_KP7        = 263,
    SDLK_KP8        = 264,
    SDLK_KP9        = 265,
    SDLK_KP_PERIOD      = 266,
    SDLK_KP_DIVIDE      = 267,
    SDLK_KP_MULTIPLY    = 268,
    SDLK_KP_MINUS       = 269,
    SDLK_KP_PLUS        = 270,
    SDLK_KP_ENTER       = 271,
    SDLK_KP_EQUALS      = 272,

    /* Arrows + Home/End pad */
    SDLK_UP         = 273,
    SDLK_DOWN       = 274,
    SDLK_RIGHT      = 275,
    SDLK_LEFT       = 276,
    SDLK_INSERT     = 277,
    SDLK_HOME       = 278,
    SDLK_END        = 279,
    SDLK_PAGEUP     = 280,
    SDLK_PAGEDOWN       = 281,

    /* Function keys */
    SDLK_F1         = 282,
    SDLK_F2         = 283,
    SDLK_F3         = 284,
    SDLK_F4         = 285,
    SDLK_F5         = 286,
    SDLK_F6         = 287,
    SDLK_F7         = 288,
    SDLK_F8         = 289,
    SDLK_F9         = 290,
    SDLK_F10        = 291,
    SDLK_F11        = 292,
    SDLK_F12        = 293,
    SDLK_F13        = 294,
    SDLK_F14        = 295,
    SDLK_F15        = 296,

    /* Key state modifier keys */
    SDLK_NUMLOCK        = 300,
    SDLK_CAPSLOCK       = 301,
    SDLK_SCROLLOCK      = 302,
    SDLK_RSHIFT     = 303,
    SDLK_LSHIFT     = 304,
    SDLK_RCTRL      = 305,
    SDLK_LCTRL      = 306,
    SDLK_RALT       = 307,
    SDLK_LALT       = 308,
    SDLK_RMETA      = 309,
    SDLK_LMETA      = 310,
    SDLK_LSUPER     = 311,      /* Left "Windows" key */
    SDLK_RSUPER     = 312,      /* Right "Windows" key */
    SDLK_MODE       = 313,      /* "Alt Gr" key */
    SDLK_COMPOSE        = 314,      /* Multi-key compose key */

    /* Miscellaneous function keys */
    SDLK_HELP       = 315,
    SDLK_PRINT      = 316,
    SDLK_SYSREQ     = 317,
    SDLK_BREAK      = 318,
    SDLK_MENU       = 319,
    SDLK_POWER      = 320,      /* Power Macintosh power key */
    SDLK_EURO       = 321,      /* Some european keyboards */
    SDLK_UNDO       = 322,      /* Atari keyboard has Undo */

    /* Add any other keys here */

    SDLK_LAST
} SDLKey;
typedef enum {
    KMOD_NONE  = 0x0000,
    KMOD_LSHIFT= 0x0001,
    KMOD_RSHIFT= 0x0002,
    KMOD_LCTRL = 0x0040,
    KMOD_RCTRL = 0x0080,
    KMOD_LALT  = 0x0100,
    KMOD_RALT  = 0x0200,
    KMOD_LMETA = 0x0400,
    KMOD_RMETA = 0x0800,
    KMOD_NUM   = 0x1000,
    KMOD_CAPS  = 0x2000,
    KMOD_MODE  = 0x4000,
    KMOD_RESERVED = 0x8000
} SDLMod;
typedef enum SDL_bool {
    SDL_FALSE = 0,
    SDL_TRUE  = 1
} SDL_bool;
typedef enum {
    DUMMY_ENUM_VALUE
} SDL_DUMMY_ENUM;
typedef enum {
    SDL_SYSWM_X11
} SDL_SYSWM_TYPE;
typedef enum {
    SDL_GL_RED_SIZE,
    SDL_GL_GREEN_SIZE,
    SDL_GL_BLUE_SIZE,
    SDL_GL_ALPHA_SIZE,
    SDL_GL_BUFFER_SIZE,
    SDL_GL_DOUBLEBUFFER,
    SDL_GL_DEPTH_SIZE,
    SDL_GL_STENCIL_SIZE,
    SDL_GL_ACCUM_RED_SIZE,
    SDL_GL_ACCUM_GREEN_SIZE,
    SDL_GL_ACCUM_BLUE_SIZE,
    SDL_GL_ACCUM_ALPHA_SIZE,
    SDL_GL_STEREO,
    SDL_GL_MULTISAMPLEBUFFERS,
    SDL_GL_MULTISAMPLESAMPLES,
    SDL_GL_ACCELERATED_VISUAL,
    SDL_GL_SWAP_CONTROL
} SDL_GLattr;
typedef enum {
    SDL_GRAB_QUERY = -1,
    SDL_GRAB_OFF = 0,
    SDL_GRAB_ON = 1,
    SDL_GRAB_FULLSCREEN /* Used internally */
} SDL_GrabMode;
#endif
SDL_WRAP_FUNC(sdl, int, SDL_Init, (Uint32 flags));
SDL_WRAP_FUNC(sdl, int, SDL_InitSubSystem, (Uint32 flags));
SDL_WRAP_FUNC(sdl, void, SDL_QuitSubSystem, (Uint32 flags));
SDL_WRAP_FUNC(sdl, Uint32, SDL_WasInit, (Uint32 flags));
SDL_WRAP_FUNC(sdl, void, SDL_Quit, (void));
SDL_WRAP_FUNC(sdl, Uint8, SDL_GetAppState, (void));
SDL_WRAP_FUNC(sdl, int, SDL_AudioInit, (const char *driver_name));
SDL_WRAP_FUNC(sdl, void, SDL_AudioQuit, (void));
SDL_WRAP_FUNC(sdl, char *, SDL_AudioDriverName, (char *namebuf, int maxlen));
SDL_WRAP_FUNC(sdl, int, SDL_OpenAudio, (SDL_AudioSpec *desired, SDL_AudioSpec *obtained));
SDL_WRAP_FUNC(sdl, SDL_audiostatus, SDL_GetAudioStatus, (void));
SDL_WRAP_FUNC(sdl, void, SDL_PauseAudio, (int pause_on));
SDL_WRAP_FUNC(sdl, SDL_AudioSpec *, SDL_LoadWAV_RW, (SDL_RWops *src, int freesrc, SDL_AudioSpec *spec, Uint8 **audio_buf, Uint32 *audio_len));
SDL_WRAP_FUNC(sdl, void, SDL_FreeWAV, (Uint8 *audio_buf));
SDL_WRAP_FUNC(sdl, int, SDL_BuildAudioCVT, (SDL_AudioCVT *cvt,
        Uint16 src_format, Uint8 src_channels, int src_rate,
        Uint16 dst_format, Uint8 dst_channels, int dst_rate));
SDL_WRAP_FUNC(sdl, int, SDL_ConvertAudio, (SDL_AudioCVT *cvt));
SDL_WRAP_FUNC(sdl, void, SDL_MixAudio, (Uint8 *dst, const Uint8 *src, Uint32 len, int volume));
SDL_WRAP_FUNC(sdl, void, SDL_LockAudio, (void));
SDL_WRAP_FUNC(sdl, void, SDL_UnlockAudio, (void));
SDL_WRAP_FUNC(sdl, void, SDL_CloseAudio, (void));
SDL_WRAP_FUNC(sdl, int, SDL_CDNumDrives, (void));
SDL_WRAP_FUNC(sdl, const char *, SDL_CDName, (int drive));
SDL_WRAP_FUNC(sdl, SDL_CD *, SDL_CDOpen, (int drive));
SDL_WRAP_FUNC(sdl, CDstatus, SDL_CDStatus, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, int, SDL_CDPlayTracks, (SDL_CD *cdrom,
        int start_track, int start_frame, int ntracks, int nframes));
SDL_WRAP_FUNC(sdl, int, SDL_CDPlay, (SDL_CD *cdrom, int start, int length));
SDL_WRAP_FUNC(sdl, int, SDL_CDPause, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, int, SDL_CDResume, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, int, SDL_CDStop, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, int, SDL_CDEject, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, void, SDL_CDClose, (SDL_CD *cdrom));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasRDTSC, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasMMX, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasMMXExt, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_Has3DNow, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_Has3DNowExt, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasSSE, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasSSE2, (void));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_HasAltiVec, (void));
SDL_WRAP_FUNC(sdl, void, SDL_SetError, (const char *fmt, ...));
SDL_WRAP_FUNC(sdl, char *, SDL_GetError, (void));
SDL_WRAP_FUNC(sdl, void, SDL_ClearError, (void));
SDL_WRAP_FUNC(sdl, void, SDL_Error, (SDL_errorcode code));
SDL_WRAP_FUNC(sdl, void, SDL_PumpEvents, (void));
SDL_WRAP_FUNC(sdl, int, SDL_PeepEvents, (SDL_Event *events, int numevents,
                SDL_eventaction action, Uint32 mask));
SDL_WRAP_FUNC(sdl, int, SDL_PollEvent, (SDL_Event *event));
SDL_WRAP_FUNC(sdl, int, SDL_WaitEvent, (SDL_Event *event));
SDL_WRAP_FUNC(sdl, int, SDL_PushEvent, (SDL_Event *event));
SDL_WRAP_FUNC(sdl, void, SDL_SetEventFilter, (SDL_EventFilter filter));
SDL_WRAP_FUNC(sdl, SDL_EventFilter, SDL_GetEventFilter, (void));
SDL_WRAP_FUNC(sdl, Uint8, SDL_EventState, (Uint8 type, int state));
SDL_WRAP_FUNC(sdl, int, SDL_NumJoysticks, (void));
SDL_WRAP_FUNC(sdl, const char *, SDL_JoystickName, (int device_index));
SDL_WRAP_FUNC(sdl, SDL_Joystick *, SDL_JoystickOpen, (int device_index));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickOpened, (int device_index));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickIndex, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickNumAxes, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickNumBalls, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickNumHats, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickNumButtons, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, void, SDL_JoystickUpdate, (void));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickEventState, (int state));
SDL_WRAP_FUNC(sdl, Sint16, SDL_JoystickGetAxis, (SDL_Joystick *joystick, int axis));
SDL_WRAP_FUNC(sdl, Uint8, SDL_JoystickGetHat, (SDL_Joystick *joystick, int hat));
SDL_WRAP_FUNC(sdl, int, SDL_JoystickGetBall, (SDL_Joystick *joystick, int ball, int *dx, int *dy));
SDL_WRAP_FUNC(sdl, Uint8, SDL_JoystickGetButton, (SDL_Joystick *joystick, int button));
SDL_WRAP_FUNC(sdl, void, SDL_JoystickClose, (SDL_Joystick *joystick));
SDL_WRAP_FUNC(sdl, int, SDL_EnableUNICODE, (int enable));
SDL_WRAP_FUNC(sdl, int, SDL_EnableKeyRepeat, (int delay, int interval));
SDL_WRAP_FUNC(sdl, void, SDL_GetKeyRepeat, (int *delay, int *interval));
SDL_WRAP_FUNC(sdl, Uint8 *, SDL_GetKeyState, (int *numkeys));
SDL_WRAP_FUNC(sdl, SDLMod, SDL_GetModState, (void));
SDL_WRAP_FUNC(sdl, void, SDL_SetModState, (SDLMod modstate));
SDL_WRAP_FUNC(sdl, char *, SDL_GetKeyName, (SDLKey key));
SDL_WRAP_FUNC(sdl, void *, SDL_LoadObject, (const char *sofile));
SDL_WRAP_FUNC(sdl, void *, SDL_LoadFunction, (void *handle, const char *name));
SDL_WRAP_FUNC(sdl, void, SDL_UnloadObject, (void *handle));
SDL_WRAP_FUNC(sdl, void, SDL_SetModuleHandle, (void *hInst));
SDL_WRAP_FUNC(sdl, int, SDL_RegisterApp, (char *name, Uint32 style, void *hInst));
SDL_WRAP_FUNC(sdl, void, SDL_UnregisterApp, (void));
SDL_WRAP_FUNC(sdl, void, SDL_InitQuickDraw, (struct QDGlobals *the_qd));
SDL_WRAP_FUNC(sdl, Uint8, SDL_GetMouseState, (int *x, int *y));
SDL_WRAP_FUNC(sdl, Uint8, SDL_GetRelativeMouseState, (int *x, int *y));
SDL_WRAP_FUNC(sdl, void, SDL_WarpMouse, (Uint16 x, Uint16 y));
SDL_WRAP_FUNC(sdl, SDL_Cursor *, SDL_CreateCursor, (Uint8 *data, Uint8 *mask, int w, int h, int hot_x, int hot_y));
SDL_WRAP_FUNC(sdl, void, SDL_SetCursor, (SDL_Cursor *cursor));
SDL_WRAP_FUNC(sdl, SDL_Cursor *, SDL_GetCursor, (void));
SDL_WRAP_FUNC(sdl, void, SDL_FreeCursor, (SDL_Cursor *cursor));
SDL_WRAP_FUNC(sdl, int, SDL_ShowCursor, (int toggle));
SDL_WRAP_FUNC(sdl, SDL_mutex *, SDL_CreateMutex, (void));
SDL_WRAP_FUNC(sdl, int, SDL_mutexP, (SDL_mutex *mutex));
SDL_WRAP_FUNC(sdl, int, SDL_mutexV, (SDL_mutex *mutex));
SDL_WRAP_FUNC(sdl, void, SDL_DestroyMutex, (SDL_mutex *mutex));
SDL_WRAP_FUNC(sdl, SDL_sem *, SDL_CreateSemaphore, (Uint32 initial_value));
SDL_WRAP_FUNC(sdl, void, SDL_DestroySemaphore, (SDL_sem *sem));
SDL_WRAP_FUNC(sdl, int, SDL_SemWait, (SDL_sem *sem));
SDL_WRAP_FUNC(sdl, int, SDL_SemTryWait, (SDL_sem *sem));
SDL_WRAP_FUNC(sdl, int, SDL_SemWaitTimeout, (SDL_sem *sem, Uint32 ms));
SDL_WRAP_FUNC(sdl, int, SDL_SemPost, (SDL_sem *sem));
SDL_WRAP_FUNC(sdl, Uint32, SDL_SemValue, (SDL_sem *sem));
SDL_WRAP_FUNC(sdl, SDL_cond *, SDL_CreateCond, (void));
SDL_WRAP_FUNC(sdl, void, SDL_DestroyCond, (SDL_cond *cond));
SDL_WRAP_FUNC(sdl, int, SDL_CondSignal, (SDL_cond *cond));
SDL_WRAP_FUNC(sdl, int, SDL_CondBroadcast, (SDL_cond *cond));
SDL_WRAP_FUNC(sdl, int, SDL_CondWait, (SDL_cond *cond, SDL_mutex *mut));
SDL_WRAP_FUNC(sdl, int, SDL_CondWaitTimeout, (SDL_cond *cond, SDL_mutex *mutex, Uint32 ms));
SDL_WRAP_FUNC(sdl, SDL_RWops *, SDL_RWFromFile, (const char *file, const char *mode));
SDL_WRAP_FUNC(sdl, SDL_RWops *, SDL_RWFromFP, (FILE *fp, int autoclose));
SDL_WRAP_FUNC(sdl, SDL_RWops *, SDL_RWFromMem, (void *mem, int size));
SDL_WRAP_FUNC(sdl, SDL_RWops *, SDL_RWFromConstMem, (const void *mem, int size));
SDL_WRAP_FUNC(sdl, SDL_RWops *, SDL_AllocRW, (void));
SDL_WRAP_FUNC(sdl, void, SDL_FreeRW, (SDL_RWops *area));
SDL_WRAP_FUNC(sdl, Uint16, SDL_ReadLE16, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, Uint16, SDL_ReadBE16, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, Uint32, SDL_ReadLE32, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, Uint32, SDL_ReadBE32, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, Uint64, SDL_ReadLE64, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, Uint64, SDL_ReadBE64, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl, int, SDL_WriteLE16, (SDL_RWops *dst, Uint16 value));
SDL_WRAP_FUNC(sdl, int, SDL_WriteBE16, (SDL_RWops *dst, Uint16 value));
SDL_WRAP_FUNC(sdl, int, SDL_WriteLE32, (SDL_RWops *dst, Uint32 value));
SDL_WRAP_FUNC(sdl, int, SDL_WriteBE32, (SDL_RWops *dst, Uint32 value));
SDL_WRAP_FUNC(sdl, int, SDL_WriteLE64, (SDL_RWops *dst, Uint64 value));
SDL_WRAP_FUNC(sdl, int, SDL_WriteBE64, (SDL_RWops *dst, Uint64 value));
SDL_WRAP_FUNC(sdl, void *, SDL_malloc, (size_t size));
SDL_WRAP_FUNC(sdl, void *, SDL_calloc, (size_t nmemb, size_t size));
SDL_WRAP_FUNC(sdl, void *, SDL_realloc, (void *mem, size_t size));
SDL_WRAP_FUNC(sdl, void, SDL_free, (void *mem));
SDL_WRAP_FUNC(sdl, char *, SDL_getenv, (const char *name));
SDL_WRAP_FUNC(sdl, int, SDL_putenv, (const char *variable));
SDL_WRAP_FUNC(sdl, void *, SDL_memset, (void *dst, int c, size_t len));
SDL_WRAP_FUNC(sdl, void *, SDL_memcpy, (void *dst, const void *src, size_t len));
SDL_WRAP_FUNC(sdl, void *, SDL_revcpy, (void *dst, const void *src, size_t len));
SDL_WRAP_FUNC(sdl, int, SDL_memcmp, (const void *s1, const void *s2, size_t len));
SDL_WRAP_FUNC(sdl, size_t, SDL_strlen, (const char *string));
SDL_WRAP_FUNC(sdl, size_t, SDL_strlcpy, (char *dst, const char *src, size_t maxlen));
SDL_WRAP_FUNC(sdl, size_t, SDL_strlcat, (char *dst, const char *src, size_t maxlen));
SDL_WRAP_FUNC(sdl, char *, SDL_strdup, (const char *string));
SDL_WRAP_FUNC(sdl, char *, SDL_strrev, (char *string));
SDL_WRAP_FUNC(sdl, char *, SDL_strupr, (char *string));
SDL_WRAP_FUNC(sdl, char *, SDL_strlwr, (char *string));
SDL_WRAP_FUNC(sdl, char *, SDL_strchr, (const char *string, int c));
SDL_WRAP_FUNC(sdl, char *, SDL_strrchr, (const char *string, int c));
SDL_WRAP_FUNC(sdl, char *, SDL_strstr, (const char *haystack, const char *needle));
SDL_WRAP_FUNC(sdl, char *, SDL_ltoa, (long value, char *string, int radix));
SDL_WRAP_FUNC(sdl, char *, SDL_ultoa, (unsigned long value, char *string, int radix));
SDL_WRAP_FUNC(sdl, long, SDL_strtol, (const char *string, char **endp, int base));
SDL_WRAP_FUNC(sdl, unsigned long, SDL_strtoul, (const char *string, char **endp, int base));
SDL_WRAP_FUNC(sdl, char*, SDL_lltoa, (Sint64 value, char *string, int radix));
SDL_WRAP_FUNC(sdl, char*, SDL_ulltoa, (Uint64 value, char *string, int radix));
SDL_WRAP_FUNC(sdl, Sint64, SDL_strtoll, (const char *string, char **endp, int base));
SDL_WRAP_FUNC(sdl, Uint64, SDL_strtoull, (const char *string, char **endp, int base));
SDL_WRAP_FUNC(sdl, double, SDL_strtod, (const char *string, char **endp));
SDL_WRAP_FUNC(sdl, int, SDL_strcmp, (const char *str1, const char *str2));
SDL_WRAP_FUNC(sdl, int, SDL_strncmp, (const char *str1, const char *str2, size_t maxlen));
SDL_WRAP_FUNC(sdl, int, SDL_strcasecmp, (const char *str1, const char *str2));
SDL_WRAP_FUNC(sdl, int, SDL_strncasecmp, (const char *str1, const char *str2, size_t maxlen));
SDL_WRAP_FUNC(sdl, int, SDL_sscanf, (const char *text, const char *fmt, ...));
SDL_WRAP_FUNC(sdl, int, SDL_snprintf, (char *text, size_t maxlen, const char *fmt, ...));
SDL_WRAP_FUNC(sdl, int, SDL_vsnprintf, (char *text, size_t maxlen, const char *fmt, va_list ap));
SDL_WRAP_FUNC(sdl, SDL_iconv_t, SDL_iconv_open, (const char *tocode, const char *fromcode));
SDL_WRAP_FUNC(sdl, int, SDL_iconv_close, (SDL_iconv_t cd));
SDL_WRAP_FUNC(sdl, size_t, SDL_iconv, (SDL_iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft));
SDL_WRAP_FUNC(sdl, char *, SDL_iconv_string, (const char *tocode, const char *fromcode, const char *inbuf, size_t inbytesleft));
SDL_WRAP_FUNC(sdl, int, SDL_GetWMInfo, (SDL_SysWMinfo *info));
SDL_WRAP_FUNC(sdl, Uint32, SDL_ThreadID, (void));
SDL_WRAP_FUNC(sdl, Uint32, SDL_GetThreadID, (SDL_Thread *thread));
SDL_WRAP_FUNC(sdl, void, SDL_WaitThread, (SDL_Thread *thread, int *status));
SDL_WRAP_FUNC(sdl, void, SDL_KillThread, (SDL_Thread *thread));
SDL_WRAP_FUNC(sdl, Uint32, SDL_GetTicks, (void));
SDL_WRAP_FUNC(sdl, void, SDL_Delay, (Uint32 ms));
SDL_WRAP_FUNC(sdl, int, SDL_SetTimer, (Uint32 interval, SDL_TimerCallback callback));
SDL_WRAP_FUNC(sdl, SDL_TimerID, SDL_AddTimer, (Uint32 interval, SDL_NewTimerCallback callback, void *param));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_RemoveTimer, (SDL_TimerID t));
SDL_WRAP_FUNC(sdl, const SDL_version *, SDL_Linked_Version, (void));
SDL_WRAP_FUNC(sdl, int, SDL_VideoInit, (const char *driver_name, Uint32 flags));
SDL_WRAP_FUNC(sdl, void, SDL_VideoQuit, (void));
SDL_WRAP_FUNC(sdl, char *, SDL_VideoDriverName, (char *namebuf, int maxlen));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_GetVideoSurface, (void));
SDL_WRAP_FUNC(sdl, const SDL_VideoInfo *, SDL_GetVideoInfo, (void));
SDL_WRAP_FUNC(sdl, int, SDL_VideoModeOK, (int width, int height, int bpp, Uint32 flags));
SDL_WRAP_FUNC(sdl, SDL_Rect **, SDL_ListModes, (SDL_PixelFormat *format, Uint32 flags));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_SetVideoMode, (int width, int height, int bpp, Uint32 flags));
SDL_WRAP_FUNC(sdl, void, SDL_UpdateRects, (SDL_Surface *screen, int numrects, SDL_Rect *rects));
SDL_WRAP_FUNC(sdl, void, SDL_UpdateRect, (SDL_Surface *screen, Sint32 x, Sint32 y, Uint32 w, Uint32 h));
SDL_WRAP_FUNC(sdl, int, SDL_Flip, (SDL_Surface *screen));
SDL_WRAP_FUNC(sdl, int, SDL_SetGamma, (float red, float green, float blue));
SDL_WRAP_FUNC(sdl, int, SDL_SetGammaRamp, (const Uint16 *red, const Uint16 *green, const Uint16 *blue));
SDL_WRAP_FUNC(sdl, int, SDL_GetGammaRamp, (Uint16 *red, Uint16 *green, Uint16 *blue));
SDL_WRAP_FUNC(sdl, int, SDL_SetColors, (SDL_Surface *surface,
            SDL_Color *colors, int firstcolor, int ncolors));
SDL_WRAP_FUNC(sdl, int, SDL_SetPalette, (SDL_Surface *surface, int flags,
                   SDL_Color *colors, int firstcolor,
                   int ncolors));
SDL_WRAP_FUNC(sdl, Uint32, SDL_MapRGB, (const SDL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b));
SDL_WRAP_FUNC(sdl, Uint32, SDL_MapRGBA, (const SDL_PixelFormat * const format,
 const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a));
SDL_WRAP_FUNC(sdl, void, SDL_GetRGB, (Uint32 pixel, SDL_PixelFormat *fmt,
                Uint8 *r, Uint8 *g, Uint8 *b));
SDL_WRAP_FUNC(sdl, void, SDL_GetRGBA, (Uint32 pixel, SDL_PixelFormat *fmt,
                 Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_CreateRGBSurface, (Uint32 flags, int width, int height, int depth,
            Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_CreateRGBSurfaceFrom, (void *pixels,
            int width, int height, int depth, int pitch,
            Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask));
SDL_WRAP_FUNC(sdl, void, SDL_FreeSurface, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, int, SDL_LockSurface, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, void, SDL_UnlockSurface, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_LoadBMP_RW, (SDL_RWops *src, int freesrc));
SDL_WRAP_FUNC(sdl, int, SDL_SaveBMP_RW, (SDL_Surface *surface, SDL_RWops *dst, int freedst));
SDL_WRAP_FUNC(sdl, int, SDL_SetColorKey, (SDL_Surface *surface, Uint32 flag, Uint32 key));
SDL_WRAP_FUNC(sdl, int, SDL_SetAlpha, (SDL_Surface *surface, Uint32 flag, Uint8 alpha));
SDL_WRAP_FUNC(sdl, SDL_bool, SDL_SetClipRect, (SDL_Surface *surface, const SDL_Rect *rect));
SDL_WRAP_FUNC(sdl, void, SDL_GetClipRect, (SDL_Surface *surface, SDL_Rect *rect));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_ConvertSurface, (SDL_Surface *src, SDL_PixelFormat *fmt, Uint32 flags));
SDL_WRAP_FUNC(sdl, int, SDL_UpperBlit, (SDL_Surface *src, SDL_Rect *srcrect,
             SDL_Surface *dst, SDL_Rect *dstrect));
SDL_WRAP_FUNC(sdl, int, SDL_LowerBlit, (SDL_Surface *src, SDL_Rect *srcrect,
             SDL_Surface *dst, SDL_Rect *dstrect));
SDL_WRAP_FUNC(sdl, int, SDL_FillRect, (SDL_Surface *dst, SDL_Rect *dstrect, Uint32 color));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_DisplayFormat, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, SDL_Surface *, SDL_DisplayFormatAlpha, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, SDL_Overlay *, SDL_CreateYUVOverlay, (int width, int height,
                Uint32 format, SDL_Surface *display));
SDL_WRAP_FUNC(sdl, int, SDL_LockYUVOverlay, (SDL_Overlay *overlay));
SDL_WRAP_FUNC(sdl, void, SDL_UnlockYUVOverlay, (SDL_Overlay *overlay));
SDL_WRAP_FUNC(sdl, int, SDL_DisplayYUVOverlay, (SDL_Overlay *overlay, SDL_Rect *dstrect));
SDL_WRAP_FUNC(sdl, void, SDL_FreeYUVOverlay, (SDL_Overlay *overlay));
SDL_WRAP_FUNC(sdl, int, SDL_GL_LoadLibrary, (const char *path));
SDL_WRAP_FUNC(sdl, void *, SDL_GL_GetProcAddress, (const char* proc));
SDL_WRAP_FUNC(sdl, int, SDL_GL_SetAttribute, (SDL_GLattr attr, int value));
SDL_WRAP_FUNC(sdl, int, SDL_GL_GetAttribute, (SDL_GLattr attr, int* value));
SDL_WRAP_FUNC(sdl, void, SDL_GL_SwapBuffers, (void));
SDL_WRAP_FUNC(sdl, void, SDL_GL_UpdateRects, (int numrects, SDL_Rect* rects));
SDL_WRAP_FUNC(sdl, void, SDL_GL_Lock, (void));
SDL_WRAP_FUNC(sdl, void, SDL_GL_Unlock, (void));
SDL_WRAP_FUNC(sdl, void, SDL_WM_SetCaption, (const char *title, const char *icon));
SDL_WRAP_FUNC(sdl, void, SDL_WM_GetCaption, (char **title, char **icon));
SDL_WRAP_FUNC(sdl, void, SDL_WM_SetIcon, (SDL_Surface *icon, Uint8 *mask));
SDL_WRAP_FUNC(sdl, int, SDL_WM_IconifyWindow, (void));
SDL_WRAP_FUNC(sdl, int, SDL_WM_ToggleFullScreen, (SDL_Surface *surface));
SDL_WRAP_FUNC(sdl, SDL_GrabMode, SDL_WM_GrabInput, (SDL_GrabMode mode));
SDL_WRAP_FUNC(sdl, int, SDL_SoftStretch, (SDL_Surface *src, SDL_Rect *srcrect,
                                    SDL_Surface *dst, SDL_Rect *dstrect));
SDL_WRAP_FUNC(sdl_image, const SDL_version *, IMG_Linked_Version, (void));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadTyped_RW, (SDL_RWops *src, int freesrc, char *type));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_Load, (const char *file));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_Load_RW, (SDL_RWops *src, int freesrc));
SDL_WRAP_FUNC(sdl_image, int, IMG_InvertAlpha, (int on));
SDL_WRAP_FUNC(sdl_image, int, IMG_isBMP, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isGIF, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isJPG, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isLBM, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isPCX, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isPNG, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isPNM, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isTIF, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isXCF, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isXPM, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, int, IMG_isXV, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadBMP_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadGIF_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadJPG_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadLBM_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadPCX_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadPNG_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadPNM_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadTGA_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadTIF_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadXCF_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadXPM_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_LoadXV_RW, (SDL_RWops *src));
SDL_WRAP_FUNC(sdl_image, SDL_Surface *, IMG_ReadXPMFromArray, (char **xpm));
SDL_WRAP_FUNC(sdl_ttf, const SDL_version *, TTF_Linked_Version, (void));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_ByteSwappedUNICODE, (int swapped));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_Init, (void));
SDL_WRAP_FUNC(sdl_ttf, TTF_Font *, TTF_OpenFont, (const char *file, int ptsize));
SDL_WRAP_FUNC(sdl_ttf, TTF_Font *, TTF_OpenFontIndex, (const char *file, int ptsize, long index));
SDL_WRAP_FUNC(sdl_ttf, TTF_Font *, TTF_OpenFontRW, (SDL_RWops *src, int freesrc, int ptsize));
SDL_WRAP_FUNC(sdl_ttf, TTF_Font *, TTF_OpenFontIndexRW, (SDL_RWops *src, int freesrc, int ptsize, long index));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GetFontStyle, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_SetFontStyle, (TTF_Font *font, int style));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GetFontOutline, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_SetFontOutline, (TTF_Font *font, int outline));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GetFontHinting, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_SetFontHinting, (TTF_Font *font, int hinting));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_FontHeight, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_FontAscent, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_FontDescent, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_FontLineSkip, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GetFontKerning, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_SetFontKerning, (TTF_Font *font, int allowed));
SDL_WRAP_FUNC(sdl_ttf, long, TTF_FontFaces, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_FontFaceIsFixedWidth, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, char *, TTF_FontFaceFamilyName, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, char *, TTF_FontFaceStyleName, (const TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GlyphIsProvided, (const TTF_Font *font, Uint16 ch));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_GlyphMetrics, (TTF_Font *font, Uint16 ch,
                     int *minx, int *maxx,
                                     int *miny, int *maxy, int *advance));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_SizeText, (TTF_Font *font, const char *text, int *w, int *h));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_SizeUTF8, (TTF_Font *font, const char *text, int *w, int *h));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_SizeUNICODE, (TTF_Font *font, const Uint16 *text, int *w, int *h));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderText_Solid, (TTF_Font *font,
                const char *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUTF8_Solid, (TTF_Font *font,
                const char *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUNICODE_Solid, (TTF_Font *font,
                const Uint16 *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderGlyph_Solid, (TTF_Font *font,
                    Uint16 ch, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderText_Shaded, (TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUTF8_Shaded, (TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUNICODE_Shaded, (TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderGlyph_Shaded, (TTF_Font *font,
                Uint16 ch, SDL_Color fg, SDL_Color bg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderText_Blended, (TTF_Font *font,
                const char *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUTF8_Blended, (TTF_Font *font,
                const char *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderUNICODE_Blended, (TTF_Font *font,
                const Uint16 *text, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, SDL_Surface *, TTF_RenderGlyph_Blended, (TTF_Font *font,
                        Uint16 ch, SDL_Color fg));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_CloseFont, (TTF_Font *font));
SDL_WRAP_FUNC(sdl_ttf, void, TTF_Quit, (void));
SDL_WRAP_FUNC(sdl_ttf, int, TTF_WasInit, (void));
