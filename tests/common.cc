#include "common.h"
#include "tkc/utf8.h"
#include "gtest/gtest.h"

#define PRINTF printf

#include <string>

using std::string;

void assert_str_eq(const wchar_t* wstr, const char* utf8) {
  char str[256];

  tk_utf8_from_utf16(wstr, str, sizeof(str));
  ASSERT_EQ(string(str), string(utf8));
}

