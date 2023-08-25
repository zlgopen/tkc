# -*- coding:utf-8 -*-

import os
import sys
import os.path

sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), 'scripts'))
import compile_config
complie_helper = compile_config.get_curr_config_for_awtk()

from awtk_config_common import OS_NAME, TARGET_ARCH, TOOLS_PREFIX, TK_SRC, TK_BIN_DIR, TK_LIB_DIR, TK_3RD_ROOT, TK_TOOLS_ROOT, OS_DEBUG, TK_DEMO_ROOT, GTEST_ROOT, TKC_STATIC_LIBS, TOOLS_NAME, NANOVG_BACKEND, NATIVE_WINDOW, TK_ROOT
from awtk_config_common import joinPath, toWholeArchive, genIdlAndDefEx, setEnvSpawn,genDllLinkFlags,copySharedLib,cleanSharedLib,scons_db_check_and_remove
from awtk_config_common import OS_FLAGS, OS_LIBS, OS_LIBPATH, OS_CPPPATH, OS_LINKFLAGS, OS_SUBSYSTEM_CONSOLE, OS_SUBSYSTEM_WINDOWS, OS_PROJECTS, COMMON_CFLAGS

######################## 交叉编译时，请根据自己的情况修改以下参数：
# TOOLS_PREFIX='/opt/poky/1.7/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-'

# OS_LINKFLAGS = ''
# COMMON_CFLAGS = ' -std=gnu99 '
# OS_LIBS = ['stdc++', 'pthread', 'rt', 'm', 'dl']
# OS_FLAGS = ' -DLINUX -DHAS_PTHREAD '
# OS_LINKFLAGS = ' -Wl,-rpath=./bin -Wl,-rpath=./ '
# OS_PROJECTS = []
# # OS_FLAGS += ' -Wall -Os -fno-strict-aliasing '
# OS_FLAGS +=' -g -Wall -Os -mfloat-abi=hard -fno-strict-aliasing '
# os.environ['LD_LIBRARY_PATH'] = ''
###################################################################

TOOLS_PREFIX = complie_helper.get_unique_value('TOOLS_PREFIX', TOOLS_PREFIX)

if TOOLS_PREFIX != '' :
    OS_PROJECTS.remove('3rd/SDL/SConscript')
    OS_FLAGS = complie_helper.get_value('OS_FLAGS', '')
    OS_LIBS = complie_helper.get_value('OS_LIBS', [])
    OS_LIBPATH += complie_helper.get_value('OS_LIBPATH', [])
    OS_CPPPATH += complie_helper.get_value('OS_CPPPATH', [])
else :
    OS_FLAGS += complie_helper.get_value('OS_FLAGS', '')
    OS_LIBS += complie_helper.get_value('OS_LIBS', [])
    OS_LIBPATH += complie_helper.get_value('OS_LIBPATH', [])
    OS_CPPPATH += complie_helper.get_value('OS_CPPPATH', [])

if OS_NAME == 'Windows':
    WIN32_AWTK_RES = 'win32_res/awtk.res'
    if TARGET_ARCH == 'x86':
        WIN32_AWTK_RES = 'win32_res/awtk_x86.res'
    
    WIN32_AWTK_RES = os.path.join(compile_config.get_curr_app_root(), WIN32_AWTK_RES)
    if not os.path.exists(WIN32_AWTK_RES):
        if TARGET_ARCH == 'x86':
            WIN32_AWTK_RES = os.path.join(TK_ROOT, 'win32_res/awtk_x86.res')
        else:
            WIN32_AWTK_RES = os.path.join(TK_ROOT, 'win32_res/awtk.res')
    
    if os.path.exists(WIN32_AWTK_RES):
        OS_LINKFLAGS += '\"' + WIN32_AWTK_RES + '\" '

if OS_DEBUG :
    BUILD_DEBUG_FLAG = ' -g -O0 '
else :
    BUILD_DEBUG_FLAG = ' -Os '

if OS_NAME == 'Windows' and TOOLS_PREFIX == '':
    if TOOLS_NAME != 'mingw':
        if OS_DEBUG :
            BUILD_DEBUG_FLAG = '  /MDd -D_DEBUG -DDEBUG /DEBUG /Od '
        else :
            BUILD_DEBUG_FLAG = ' -DNDEBUG /MD /O2 /Oi  '
        
        if complie_helper.get_value('PDB') :
            BUILD_DEBUG_LINKFLAGS = ' /DEBUG '

BUILD_DEBUG_FLAG = ''
BUILD_DEBUG_LINKFLAGS = ''
AWTK_STATIC_LIBS = TKC_STATIC_LIBS

COMMON_CCFLAGS = ' -DTK_ROOT=\"\\\"'+TK_ROOT+'\\\"\" '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DTKC_ONLY=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DWITH_MBEDTLS=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DWITH_DATA_READER_WRITER=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS + \
    ' -DHAS_STDIO -DHAVE_STDIO_H -DHAS_GET_TIME_US64 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DHAS_STD_MALLOC -DTK_MAX_MEM_BLOCK_NR=3 '

SDL_LIBS=[]
if TOOLS_PREFIX == '' :
    SDL_LIBS = ['SDL2', 'glad']
    COMMON_CCFLAGS = COMMON_CCFLAGS+' -DWITH_SDL '
else :
    SDL_LIBS = []
    CC = TOOLS_PREFIX+'gcc'
    CXX = TOOLS_PREFIX+'g++'
    LD = TOOLS_PREFIX+'g++'
    AR = TOOLS_PREFIX+'ar'
    STRIP = TOOLS_PREFIX+'strip'
    RANLIB = TOOLS_PREFIX+'ranlib'

CFLAGS = COMMON_CFLAGS
LINKFLAGS = OS_LINKFLAGS
LIBPATH = [TK_LIB_DIR, TK_BIN_DIR] + OS_LIBPATH
CCFLAGS = OS_FLAGS + COMMON_CCFLAGS
AWTK_CCFLAGS = OS_FLAGS + COMMON_CCFLAGS
AWTK_DLL_DEPS_LIBS = AWTK_STATIC_LIBS + SDL_LIBS + OS_LIBS

STATIC_LIBS = AWTK_STATIC_LIBS + SDL_LIBS + OS_LIBS
SHARED_LIBS = ['tkc'] + OS_LIBS

LIBS = STATIC_LIBS

CPPPATH = [TK_ROOT,
           TK_SRC,
           TK_3RD_ROOT,
           joinPath(TK_3RD_ROOT, 'mbedtls/include'),
           joinPath(TK_3RD_ROOT, 'mbedtls/3rdparty/everest/include'),
           joinPath(TK_3RD_ROOT, 'SDL/src'),
           joinPath(TK_3RD_ROOT, 'SDL/include'),
           joinPath(TK_3RD_ROOT, 'gtest/googletest'),
           joinPath(TK_3RD_ROOT, 'gtest/googletest/include'),
           TK_TOOLS_ROOT] + OS_CPPPATH

os.environ['AWTK_DLL_DEPS_LIBS'] = ';'.join(AWTK_DLL_DEPS_LIBS)
os.environ['STATIC_LIBS'] = ';'.join(STATIC_LIBS)
os.environ['AWTK_CCFLAGS'] = AWTK_CCFLAGS + BUILD_DEBUG_FLAG
os.environ['SDL_UBUNTU_USE_IME'] = str(False)
os.environ['NATIVE_WINDOW'] = ''
os.environ['CCFLAGS'] = CCFLAGS + BUILD_DEBUG_FLAG
os.environ['CROSS_COMPILE'] = str(not TOOLS_PREFIX == '')

OS_LIBS = SDL_LIBS + OS_LIBS

def genIdlAndDef():
    genIdlAndDefEx(False)
