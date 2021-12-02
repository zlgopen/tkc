import os
import os.path
import platform
import shutil
from shutil import copyfile
from awtk_config_common import OS_NAME, TARGET_ARCH, TOOLS_PREFIX, TK_SRC, TK_BIN_DIR, TK_LIB_DIR, TK_3RD_ROOT, TK_TOOLS_ROOT, TK_DEMO_ROOT, GTEST_ROOT, TKC_STATIC_LIBS, TOOLS_NAME, NANOVG_BACKEND, NATIVE_WINDOW, TK_ROOT
from awtk_config_common import joinPath, toWholeArchive, genIdlAndDefEx, setEnvSpawn,genDllLinkFlags,copySharedLib
from awtk_config_common import OS_FLAGS, OS_LIBS, OS_LIBPATH, OS_CPPPATH, OS_LINKFLAGS, OS_SUBSYSTEM_CONSOLE, OS_SUBSYSTEM_WINDOWS, OS_PROJECTS, COMMON_CFLAGS

AWTK_STATIC_LIBS = TKC_STATIC_LIBS

COMMON_CCFLAGS = ' -DTK_ROOT=\"\\\"'+TK_ROOT+'\\\"\" '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DTKC_ONLY=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DUSE_TK_PREFIX=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DWITH_MBEDTLS=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DWITH_DATA_READER_WRITER=1 '
COMMON_CCFLAGS = COMMON_CCFLAGS + \
    ' -DWITH_SDL -DHAS_STDIO -DHAVE_STDIO_H -DHAS_GET_TIME_US64 '
COMMON_CCFLAGS = COMMON_CCFLAGS+' -DHAS_STD_MALLOC -DTK_MAX_MEM_BLOCK_NR=3 '


CFLAGS = COMMON_CFLAGS
LINKFLAGS = OS_LINKFLAGS
LIBPATH = [TK_LIB_DIR, TK_BIN_DIR] + OS_LIBPATH
CCFLAGS = OS_FLAGS + COMMON_CCFLAGS
AWTK_CCFLAGS = OS_FLAGS + COMMON_CCFLAGS
AWTK_DLL_DEPS_LIBS = AWTK_STATIC_LIBS + ['SDL2', 'glad'] + OS_LIBS

STATIC_LIBS = AWTK_STATIC_LIBS + ['SDL2', 'glad'] + OS_LIBS
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
os.environ['AWTK_CCFLAGS'] = AWTK_CCFLAGS
os.environ['SDL_UBUNTU_USE_IME'] = str(False)
os.environ['NATIVE_WINDOW'] = ''
os.environ['CCFLAGS'] = CCFLAGS

OS_LIBS = ['SDL2', 'glad'] + OS_LIBS

def genIdlAndDef():
    genIdlAndDefEx(False)
