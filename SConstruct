import os
import awtk_config as awtk

awtk.scons_db_check_and_remove()

APP_TOOLS = None
if awtk.TOOLS_NAME != '' :
  APP_TOOLS = [awtk.TOOLS_NAME]

awtk.genIdlAndDef();
if awtk.TOOLS_PREFIX == '' :
  DefaultEnvironment(TOOLS = APP_TOOLS,
    CCFLAGS = awtk.AWTK_CCFLAGS,
    LIBS = awtk.LIBS,
    LIBPATH = awtk.LIBPATH,
    CPPPATH = awtk.CPPPATH + [awtk.joinPath(awtk.TK_ROOT, 'res')],
    LINKFLAGS = awtk.LINKFLAGS,
    TARGET_ARCH=awtk.TARGET_ARCH,
    OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
    OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
  )

  awtk.OS_PROJECTS += ['src/hal/tools/network_shell/SConscript']
else :
  DefaultEnvironment(
    ENV = os.environ,
    CC = awtk.CC,
    CXX= awtk.CXX,
    LD = awtk.LD,
    AR = awtk.AR,
    RANLIB = awtk.RANLIB,
    STRIP = awtk.STRIP,
    LIBS = awtk.LIBS,
    CFLAGS = awtk.CFLAGS,
    LIBPATH = awtk.LIBPATH,
    CPPPATH = awtk.CPPPATH,
    LINKFLAGS = awtk.LINKFLAGS,
    CCFLAGS = awtk.AWTK_CCFLAGS, 
    OS_SUBSYSTEM_CONSOLE = awtk.OS_SUBSYSTEM_CONSOLE,
    OS_SUBSYSTEM_WINDOWS = awtk.OS_SUBSYSTEM_WINDOWS
  )

SConscriptFiles=[
  '3rd/glad/SConscript',
  '3rd/cjson/SConscript',
  '3rd/mbedtls/SConscript',
  '3rd/miniz/SConscript',
  'src/SConscript',
  'src/csv/SConscript',
  'src/charset/SConscript',
  'src/xml/SConscript',
  'src/romfs/SConscript',
  'src/conf_io/SConscript',
  'src/hal/SConscript',
  'src/fscript_ext/SConscript',
  'src/streams/SConscript',
  'src/streams/tools/SConscript',
  'src/compressors/SConscript',
  'src/ubjson/SConscript',
  'src/debugger/SConscript',
  'tests/SConscript',
  'demos/SConscript',
  ] + awtk.OS_PROJECTS
  
os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;

SConscript(SConscriptFiles)

