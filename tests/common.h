#include "tkc/utf8.h"
#include <string>

using std::string;

#ifndef COMMON_H
#define COMMON_H

static inline void assert_str_eq(const wchar_t* wstr, const char* utf8) {
  char str[256];

  tk_utf8_from_utf16(wstr, str, sizeof(str));
  ASSERT_EQ(string(str), string(utf8));
}

#endif /*COMMON_H*/
