import os
import sys
import atexit
sys.path.insert(0, './scripts')
import compile_config
compile_helper = compile_config.compile_helper()
compile_helper.try_load_default_config()
compile_helper.scons_user_sopt(ARGUMENTS)
compile_config.set_curr_config(compile_helper)
import awtk_config as awtk

APP_TOOLS = ['default']
awtk.scons_db_check_and_remove()

if awtk.TOOLS_NAME != '' :
  if awtk.TOOLS_NAME == 'mingw':
    APP_TOOLS = ['mingw']

awtk.genIdlAndDef();
if awtk.TOOLS_PREFIX == '' :
  env = DefaultEnvironment(TOOLS = APP_TOOLS,
    CCFLAGS = awtk.AWTK_CCFLAGS + awtk.BUILD_DEBUG_FLAG,
    LIBS = awtk.LIBS,
    LIBPATH = awtk.LIBPATH,
    CPPPATH = awtk.CPPPATH + [awtk.joinPath(awtk.TK_ROOT, 'res')],
    LINKFLAGS = awtk.LINKFLAGS + awtk.BUILD_DEBUG_LINKFLAGS,
    TARGET_ARCH=awtk.TARGET_ARCH,
    OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
    OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
  )

  if awtk.OS_NAME == 'Darwin':
    env.AppendUnique(SHLINKFLAGS=['-install_name', '@rpath/$TARGET.name'])

  awtk.OS_PROJECTS += ['src/hal/tools/network_shell/SConscript']
elif awtk.OS_NAME == 'Linux' :
  DefaultEnvironment(
    ENV = os.environ,
    TOOLS=['gcc', 'g++', 'gnulink', 'ar', 'gas', 'gfortran', 'm4'],
    PROGPREFIX='',PROGSUFFIX='',LIBPREFIX='lib',LIBSUFFIX='.a',SHLIBPREFIX='lib',SHLIBSUFFIX='.so',LIBPREFIXES=['lib'],LIBSUFFIXES=['.a', '.so'],
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
else :
  print("Only support OS_NAME = Linux when TOOLS_PREFIX != NULL, Please setting OS_NAME in awtk_config_common.py")
  exit(-1)

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
  ]

if compile_helper.get_value('BUILD_DEMOS', True) :
  SConscriptFiles += ['demos/SConscript']

if compile_helper.get_value('BUILD_TESTS', True) :
  SConscriptFiles += ['tests/SConscript']

SConscriptFiles += awtk.OS_PROJECTS

os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;
os.environ['TARGET_OS'] = awtk.OS_NAME;

SConscript(SConscriptFiles)

def compile_end() :
  compile_helper.output_compile_data(awtk.TK_ROOT)

atexit.register(compile_end)
