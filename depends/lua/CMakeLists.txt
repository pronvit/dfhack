PROJECT ( lua CXX )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DLUA_USE_APICHECK")

IF(WIN32)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE )
ELSE()
    ADD_DEFINITIONS ( -DLUA_USE_POSIX -DLUA_USE_DLOPEN )
    SET ( LIBS m dl )
ENDIF()

IF(UNIX)
    ADD_DEFINITIONS(-DLINUX_BUILD)
    IF(DFHACK_BUILD_64)
        SET(CMAKE_C_FLAGS "-m64 -mno-avx")
    ELSE()
        SET(CMAKE_C_FLAGS "-m32")
    ENDIF()
ENDIF()

SET (HDR_LIBLUA
src/lapi.h
src/lauxlib.h
src/lcode.h
src/lctype.h
src/ldebug.h
src/ldo.h
src/lfunc.h
src/lgc.h
src/llex.h
src/llimits.h
src/lmem.h
src/lobject.h
src/lopcodes.h
src/lparser.h
src/lstate.h
src/lstring.h
src/ltable.h
src/ltm.h
src/lua.h
src/luaconf.h
src/lualib.h
src/lundump.h
src/lvm.h
src/lzio.h
)
SET_SOURCE_FILES_PROPERTIES(${HDR_LIBLUA} PROPERTIES HEADER_FILE_ONLY TRUE)
include_directories(src)

# Build Libraries
SET (SRC_LIBLUA
src/lapi.c
src/lauxlib.c
src/lbaselib.c
src/lbitlib.c
src/lcode.c
src/lcorolib.c
src/lctype.c
src/ldblib.c
src/ldebug.c
src/ldo.c
src/ldump.c
src/lfunc.c
src/lgc.c
src/linit.c
src/liolib.c
src/llex.c
src/lmathlib.c
src/lmem.c
src/loadlib.c
src/lobject.c
src/lopcodes.c
src/loslib.c
src/lparser.c
src/lstate.c
src/lstring.c
src/lstrlib.c
src/ltable.c
src/ltablib.c
src/ltm.c
src/lundump.c
src/lvm.c
src/lzio.c
src/lutf8lib.c
)
# compile with C++ compiler
set_source_files_properties(${SRC_LIBLUA} PROPERTIES LANGUAGE CXX)
# append headers to sources to make them show up in MSVC GUI
LIST(APPEND SRC_LIBLUA ${HDR_LIBLUA})

ADD_LIBRARY ( lua SHARED ${SRC_LIBLUA} )
TARGET_LINK_LIBRARIES ( lua ${LIBS})

install(TARGETS lua
        LIBRARY DESTINATION ${DFHACK_LIBRARY_DESTINATION}
        RUNTIME DESTINATION ${DFHACK_LIBRARY_DESTINATION})

IDE_FOLDER(lua "Depends")

#SET ( SRC_LUA src/lua.c )
#SET ( SRC_LUAC src/luac.c src/print.c )

#ADD_EXECUTABLE ( lua ${SRC_LUA} ${LUA_RC_FILE})
#ADD_EXECUTABLE ( luac ${SRC_LUAC} ${LUAC_RC_FILE})
#TARGET_LINK_LIBRARIES ( lua liblua )
#TARGET_LINK_LIBRARIES ( luac liblua_static )

