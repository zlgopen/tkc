﻿/**
 * File:   utf8.c
 * Author: AWTK Develop Team
 * Brief:  utf8 encode decode
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"

/*UTF8-related functions are copied from glib.*/

#define UTF8_COMPUTE(Char, Mask, Len) \
  if (Char < 128) {                   \
    Len = 1;                          \
    Mask = 0x7f;                      \
  } else if ((Char & 0xe0) == 0xc0) { \
    Len = 2;                          \
    Mask = 0x1f;                      \
  } else if ((Char & 0xf0) == 0xe0) { \
    Len = 3;                          \
    Mask = 0x0f;                      \
  } else if ((Char & 0xf8) == 0xf0) { \
    Len = 4;                          \
    Mask = 0x07;                      \
  } else if ((Char & 0xfc) == 0xf8) { \
    Len = 5;                          \
    Mask = 0x03;                      \
  } else if ((Char & 0xfe) == 0xfc) { \
    Len = 6;                          \
    Mask = 0x01;                      \
  } else                              \
    Len = -1;

#define UTF8_LENGTH(Char) \
  ((Char) < 0x80          \
       ? 1                \
       : ((Char) < 0x800  \
              ? 2         \
              : ((Char) < 0x10000 ? 3 : ((Char) < 0x200000 ? 4 : ((Char) < 0x4000000 ? 5 : 6)))))

#define UTF8_GET(Result, Chars, Count, Mask, Len) \
  (Result) = (Chars)[0] & (Mask);                 \
  for ((Count) = 1; (Count) < (Len); ++(Count)) { \
    if (((Chars)[(Count)] & 0xc0) != 0x80) {      \
      (Result) = -1;                              \
      break;                                      \
    }                                             \
    (Result) <<= 6;                               \
    (Result) |= ((Chars)[(Count)] & 0x3f);        \
  }

#define UNICODE_VALID(Char)                                                                        \
  ((Char) < 0x110000 && (((Char)&0xFFFFF800) != 0xD800) && ((Char) < 0xFDD0 || (Char) > 0xFDEF) && \
   ((Char)&0xFFFE) != 0xFFFE)
static const char utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1};

uint32_t tk_utf8_get_bytes_of_leading(uint8_t c) {
  return utf8_skip_data[c];
}

const char* const g_utf8_skip = utf8_skip_data;
#define g_utf8_next_char(p) (char*)((p) + g_utf8_skip[*(const unsigned char*)(p)])

static uint32_t utf8_get_char(const char* p, const char** next) {
  uint32_t mask = 0;
  uint32_t result = 0;
  int32_t i = 0, len = 0;
  unsigned char c = (unsigned char)*p;

  UTF8_COMPUTE(c, mask, len);
  if (len == -1) return (uint32_t)-1;
  UTF8_GET(result, p, i, mask, len);

  if (next != NULL) {
    *next = g_utf8_next_char(p);
  }

  return result;
}

static int unichar_to_utf8(uint32_t c, char* outbuf) {
  /* If this gets modified, also update the copy in g_string_insert_unichar() */
  size_t len = 0;
  int first;
  int i;

  if (c < 0x80) {
    first = 0;
    len = 1;
  } else if (c < 0x800) {
    first = 0xc0;
    len = 2;
  } else if (c < 0x10000) {
    first = 0xe0;
    len = 3;
  } else if (c < 0x200000) {
    first = 0xf0;
    len = 4;
  } else if (c < 0x4000000) {
    first = 0xf8;
    len = 5;
  } else {
    first = 0xfc;
    len = 6;
  }

  if (outbuf) {
    for (i = len - 1; i > 0; --i) {
      outbuf[i] = (c & 0x3f) | 0x80;
      c >>= 6;
    }
    outbuf[0] = c | first;
  }

  return len;
}

#define SURROGATE_VALUE(h, l) (((h)-0xd800) * 0x400 + (l)-0xdc00 + 0x10000)

static char* utf16_to_utf8(const wchar_t* str, int32_t len, char* utf8, int out_len) {
  /* This function and g_utf16_to_ucs4 are almost exactly identical - The lines
   * that differ are marked.
   */
  const wchar_t* in;
  char* out;
  char* result = NULL;
  int n_bytes;
  wchar_t high_surrogate;

  return_value_if_fail(str != NULL, NULL);

  n_bytes = 0;
  in = str;
  high_surrogate = 0;
  while ((len < 0 || in - str < len) && *in) {
    wchar_t c = *in;
    uint32_t wc;

    if (c >= 0xdc00 && c < 0xe000) /* low surrogate */
    {
      if (high_surrogate) {
        wc = SURROGATE_VALUE(high_surrogate, c);
        high_surrogate = 0;
      } else {
        log_error("Invalid sequence in conversion input");
        goto err_out;
      }
    } else {
      if (high_surrogate) {
        log_error("Invalid sequence in conversion input");
        goto err_out;
      }

      if (c >= 0xd800 && c < 0xdc00) /* high surrogate */
      {
        high_surrogate = c;
        goto next1;
      } else
        wc = c;
    }

    /********** DIFFERENT for UTF8/UCS4 **********/
    n_bytes += UTF8_LENGTH(wc);

  next1:
    in++;
  }

  if (high_surrogate) {
    log_error("Partial character sequence at end of input");
    goto err_out;
  }

  /* At this point, everything is valid, and we just need to convert
   */
  /********** DIFFERENT for UTF8/UCS4 **********/
  // result = g_malloc (n_bytes + 1);
  result = utf8;
  if (out_len <= n_bytes) {
    log_error("utf16_to_utf8: out_len is not big enough\n");
    goto err_out;
  }

  high_surrogate = 0;
  out = result;
  in = str;
  while (out < result + n_bytes) {
    wchar_t c = *in;
    uint32_t wc;

    if (c >= 0xdc00 && c < 0xe000) /* low surrogate */
    {
      wc = SURROGATE_VALUE(high_surrogate, c);
      high_surrogate = 0;
    } else if (c >= 0xd800 && c < 0xdc00) /* high surrogate */
    {
      high_surrogate = c;
      goto next2;
    } else
      wc = c;

    /********** DIFFERENT for UTF8/UCS4 **********/
    out += unichar_to_utf8(wc, out);

  next2:
    in++;
  }

  /********** DIFFERENT for UTF8/UCS4 **********/
  *out = '\0';

  return result;
err_out:
  return NULL;
}

char* tk_utf8_from_utf16_ex(const wchar_t* in, uint32_t in_size, char* out, uint32_t out_size) {
  return_value_if_fail(in != NULL && out != NULL && out_size > 0, NULL);
  if (in_size == 0) {
    *out = '\0';
    return out;
  }

  return utf16_to_utf8(in, in_size, out, out_size);
}

char* tk_utf8_from_utf16(const wchar_t* str, char* out, uint32_t out_size) {
  return_value_if_fail(str != NULL && out != NULL && out_size > 0, NULL);

  return utf16_to_utf8(str, wcslen(str), out, out_size);
}

wchar_t* tk_utf8_to_utf16(const char* str, wchar_t* out, uint32_t out_size) {
  return_value_if_fail(str != NULL && out != NULL && out_size > 0, NULL);

  return tk_utf8_to_utf16_ex(str, strlen(str), out, out_size);
}

wchar_t* tk_utf8_to_utf16_ex(const char* str, uint32_t size, wchar_t* out, uint32_t out_size) {
  uint32_t i = 0;
  uint32_t val = 0;
  const char* p = str;
  const char* end = NULL;
  const char* next = NULL;
  return_value_if_fail(str != NULL && out != NULL, NULL);

  end = str + size;
  while (p != NULL && p < end && (i + 1) < out_size) {
    val = utf8_get_char(p, &next);
    return_value_if_fail(val != -1, NULL);

    if (sizeof(wchar_t) == 4) {
      out[i++] = val;
    } else if (sizeof(wchar_t) == 2) {
      if (val < 0x10000) {
        out[i++] = val;
      } else {
        val -= 0x10000;
        out[i++] = 0xd800 + (val / 0x400);
        return_value_if_fail((i + 1) < out_size, NULL);
        out[i++] = 0xdc00 + (val & 0x3FF);
      }
    } else {
      return NULL;
    }

    p = next;
  }
  out[i] = '\0';

  return out;
}

char* tk_utf8_dup_utf16(const wchar_t* in, int32_t size) {
  char* out = NULL;
  uint32_t out_size = 0;
  return_value_if_fail(in != NULL, NULL);

  if (size < 0) {
    size = wcslen(in);
  }

  out_size = size * 4 + 2;
  out = TKMEM_ALLOC(out_size);
  return_value_if_fail(out != NULL, NULL);
  memset(out, 0x00, out_size);

  return tk_utf8_from_utf16_ex(in, size, out, out_size - 1);
}

char* tk_utf8_trim_invalid_char(char* str) {
  int32_t i = 0;
  int32_t n = 0;
  char* p = str;
  return_value_if_fail(str != NULL, str);

  while (*p) {
    n = tk_utf8_get_bytes_of_leading(*p);
    if (n > 0) {
      for (i = 0; i < n; i++) {
        if (p[i] == '\0') {
          *p = '\0';
          return str;
        }
      }
    }
    p += n;
  }

  return str;
}
