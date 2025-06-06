#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include <stdio.h>

#include "tkc.h"
#include "gtest/gtest.h"
#include "fscript_ext/fscript_ext.h"
#include "conf_io/app_conf.h"
#include "conf_io/app_conf_init_json.h"
#include "conf_io/app_conf_init_ubjson.h"
#include "conf_io/app_conf_init_ini.h"

GTEST_API_ int main(int argc, char** argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);

  return_value_if_fail(platform_prepare() == RET_OK, RET_FAIL);
  tk_socket_init();
  data_writer_factory_set(data_writer_factory_create());
  data_reader_factory_set(data_reader_factory_create());
  data_writer_factory_register(data_writer_factory(), "file", data_writer_file_create);
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  data_reader_factory_register(data_reader_factory(), "mem", data_reader_mem_create);
  data_writer_factory_register(data_writer_factory(), "wbuffer", data_writer_wbuffer_create);

  fscript_global_init();
  fscript_ext_init();

  int ret = RUN_ALL_TESTS();

  tk_socket_deinit();
  app_conf_deinit();
  fscript_global_deinit();
  data_writer_factory_destroy(data_writer_factory());
  data_reader_factory_destroy(data_reader_factory());
  data_writer_factory_set(NULL);
  data_reader_factory_set(NULL);

  return ret;
}
