/**
 * File:   wstr.c
 * Author: AWTK Develop Team
 * Brief:  width char
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/wstr.h"
#include "tkc/utils.h"

const wchar_t* wcs_chr(const wchar_t* s, wchar_t c) {
  const wchar_t* p = s;
  return_value_if_fail(s != NULL, NULL);

  while (*p && *p != c) p++;

  return *p ? p : NULL;
}

wchar_t* wcs_ncpy(wchar_t* s1, const wchar_t* s2, uint32_t n) {
  wchar_t* d = s1;
  const wchar_t* s = s2;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s && n-- > 0) {
    *d++ = *s++;
  }
  *d = '\0';

  return s1;
}

wchar_t* wcs_cpy(wchar_t* s1, const wchar_t* s2) {
  wchar_t* d = s1;
  const wchar_t* s = s2;
  return_value_if_fail(s1 != NULL && s2 != NULL, NULL);

  while (*s) {
    *d++ = *s++;
  }
  *d = '\0';

  return s1;
}

int wcs_ncmp(const wchar_t* s1, const wchar_t* s2, uint32_t n) {
  wchar_t c1 = L'\0';
  wchar_t c2 = L'\0';
  return_value_if_fail(s1 != NULL && s2 != NULL, -1);

  if (n >= 4) {
    size_t n4 = n >> 2;
    do {
      c1 = *s1++;
      c2 = *s2++;
      if (c1 == L'\0' || c1 != c2) {
        return c1 > c2 ? 1 : (c1 < c2 ? -1 : 0);
      }
      c1 = *s1++;
      c2 = *s2++;
      if (c1 == L'\0' || c1 != c2) {
        return c1 > c2 ? 1 : (c1 < c2 ? -1 : 0);
      }
      c1 = *s1++;
      c2 = *s2++;
      if (c1 == L'\0' || c1 != c2) {
        return c1 > c2 ? 1 : (c1 < c2 ? -1 : 0);
      }
      c1 = *s1++;
      c2 = *s2++;
      if (c1 == L'\0' || c1 != c2) {
        return c1 > c2 ? 1 : (c1 < c2 ? -1 : 0);
      }
    } while (--n4 > 0);
    n &= 3;
  }

  while (n > 0) {
    c1 = *s1++;
    c2 = *s2++;
    if (c1 == L'\0' || c1 != c2) {
      return c1 > c2 ? 1 : (c1 < c2 ? -1 : 0);
    }
    n--;
  }

  return 0;
}

int wcs_cmp(const wchar_t* s1, const wchar_t* s2) {
  uint32_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, -1);

  for (i = 0; s1[i] && s2[i]; i++) {
    int ret = s1[i] - s2[i];
    if (ret) {
      return ret;
    }
  }

  return s1[i] - s2[i];
}

int wcs_case_cmp(const wchar_t* s1, const wchar_t* s2) {
  uint32_t i = 0;
  return_value_if_fail(s1 != NULL && s2 != NULL, -1);

  for (i = 0; s1[i] && s2[i]; i++) {
    int ret = tolower(s1[i]) - tolower(s2[i]);
    if (ret) {
      return ret;
    }
  }

  return tolower(s1[i]) - tolower(s2[i]);
}

wchar_t* wcs_dup(const wchar_t* s) {
  wchar_t* s1 = TKMEM_ZALLOCN(wchar_t, wcslen(s) + 1);

  return wcs_cpy(s1, s);
}

size_t wcs_len(const wchar_t* s) {
  size_t size = 0;
  const wchar_t* p = s;
  return_value_if_fail(s != NULL, 0);

  while (*p++) {
    size++;
  }

  return size;
}

#ifdef WITH_WCSXXX
size_t wcslen(const wchar_t* s) {
  return wcs_len(s);
}

int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
  return wcs_ncmp(s1, s2, n);
}

int wcscmp(const wchar_t* s1, const wchar_t* s2) {
  return wcs_cmp(s1, s2);
}

int wcscasecmp(const wchar_t* s1, const wchar_t* s2) {
  return wcs_case_cmp(s1, s2);
}

wchar_t* wcscpy(wchar_t* s1, const wchar_t* s2) {
  return wcs_cpy(s1, s2);
}

wchar_t* wcsncpy(wchar_t* s1, const wchar_t* s2, uint32_t n) {
  return wcs_ncpy(s1, s2, n);
}

wchar_t* wcschr(const wchar_t* s, wchar_t c) {
  return (wchar_t*)wcs_chr(s, c);
}

wchar_t* wcsdup(const wchar_t* s) {
  return wcs_dup(s);
}

int iswspace(wchar_t ch) {
  return ch == ' ';
}

#endif /*WITH_WCSXXX*/

static ret_t wstr_extend(wstr_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (capacity <= str->capacity) {
    return RET_OK;
  }

  if (!str->extendable) {
    return RET_FAIL;
  }

  if (capacity > 0) {
    wchar_t* s = TKMEM_REALLOCT(wchar_t, str->str, capacity + 1);
    return_value_if_fail(s != NULL, RET_FAIL);

    s[str->size] = 0;
    str->str = s;
    str->capacity = capacity;
  }

  return RET_OK;
}

wstr_t* wstr_init(wstr_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, NULL);

  memset(str, 0x00, sizeof(wstr_t));
  str->extendable = TRUE;

  return wstr_extend(str, capacity) == RET_OK ? str : NULL;
}

wstr_t* wstr_attach(wstr_t* str, wchar_t* buff, uint32_t capacity) {
  return_value_if_fail(str != NULL && buff != NULL && capacity > 0, NULL);

  memset(str, 0x00, sizeof(str_t));
  memset(buff, 0x00, capacity * sizeof(wchar_t));

  str->str = buff;
  str->size = 0;
  str->capacity = capacity;
  str->extendable = FALSE;

  return str;
}

ret_t wstr_set(wstr_t* str, const wchar_t* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_set_with_len(str, text, 0xffffffff);
}

ret_t wstr_set_with_len(wstr_t* str, const wchar_t* text, uint32_t len) {
  uint32_t size = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  size = wcslen(text);
  size = tk_min(size, len);
  return_value_if_fail(wstr_extend(str, size + 1) == RET_OK, RET_BAD_PARAMS);

  wcsncpy(str->str, text, size);
  str->size = size;
  str->str[size] = 0;

  return RET_OK;
}

ret_t wstr_clear(wstr_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  str->size = 0;
  if (str->str != NULL) {
    str->str[0] = '\0';
  }

  return RET_OK;
}

ret_t wstr_set_utf8_with_len(wstr_t* str, const char* text, uint32_t len) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, len + 2) == RET_OK, RET_OOM);

  if (tk_utf8_to_utf16_ex(text, len, str->str, str->capacity - 1) == NULL) {
    log_warn("wstr_set_utf8_with_len fail, please ensure the parameters are UTF-8 encoded!\n");
    wstr_reset(str);
    return RET_BAD_PARAMS;
  }

  str->size = wcslen(str->str);

  return RET_OK;
}

ret_t wstr_set_utf8(wstr_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, strlen(text) + 2) == RET_OK, RET_OOM);

  if (tk_utf8_to_utf16(text, str->str, str->capacity - 1) == NULL) {
    log_warn("wstr_set_utf8 fail, please ensure the parameters are UTF-8 encoded!\n");
    wstr_reset(str);
    return RET_BAD_PARAMS;
  }

  str->size = wcslen(str->str);

  return RET_OK;
}

ret_t wstr_get_utf8(wstr_t* str, char* text, uint32_t size) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  tk_utf8_from_utf16(str->str, text, size);

  return RET_OK;
}

ret_t wstr_remove(wstr_t* str, uint32_t offset, uint32_t nr) {
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && offset < str->size && nr > 0, RET_BAD_PARAMS);

  if ((offset + nr) > str->size) {
    nr = str->size - offset;
  }

  if (nr > 0) {
    p = str->str;
    memmove(p + offset, p + offset + nr, (str->size - offset - nr) * sizeof(wchar_t));
    str->size -= nr;
    str->str[str->size] = 0;
  }

  return RET_OK;
}

ret_t wstr_insert(wstr_t* str, uint32_t offset, const wchar_t* text, uint32_t nr) {
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && text != NULL && offset <= str->size && nr > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, str->size + nr + 1) == RET_OK, RET_BAD_PARAMS);

  p = str->str;
  if (str->size > offset) {
    uint32_t i = 0;
    uint32_t s = str->size - 1;
    uint32_t d = str->size + nr - 1;
    uint32_t move_nr = str->size - offset;

    for (i = 0; i < move_nr; i++, d--, s--) {
      p[d] = p[s];
    }
  }
  memcpy(p + offset, text, nr * sizeof(wchar_t));
  str->size += nr;
  str->str[str->size] = 0;

  return RET_OK;
}

ret_t wstr_append_with_len(wstr_t* str, const wchar_t* text, uint32_t size) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return_value_if_fail(wstr_extend(str, str->size + size + 1) == RET_OK, RET_BAD_PARAMS);

  memcpy(str->str + str->size, text, size * sizeof(wchar_t));
  str->size += size;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t wstr_append(wstr_t* str, const wchar_t* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return wstr_append_with_len(str, text, wcslen(text));
}

ret_t wstr_push(wstr_t* str, const wchar_t c) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wstr_extend(str, str->size + 2) == RET_OK, RET_BAD_PARAMS);
  str->str[str->size++] = c;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t wstr_append_int(wstr_t* str, int32_t value) {
  return wstr_push_int(str, "%d", value);
}

ret_t wstr_push_int(wstr_t* str, const char* format, int32_t value) {
  char buff[TK_NUM_MAX_LEN + 1];
  wchar_t s[TK_NUM_MAX_LEN + 1];

  memset(s, 0x00, sizeof(s));
  memset(buff, 0x00, sizeof(buff));
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);

  tk_snprintf(buff, TK_NUM_MAX_LEN, format, value);
  tk_utf8_to_utf16(buff, s, TK_NUM_MAX_LEN);

  return wstr_append_with_len(str, s, wcslen(s));
}

ret_t wstr_pop(wstr_t* str) {
  return_value_if_fail(str != NULL && str->size > 0, RET_BAD_PARAMS);
  str->size--;
  str->str[str->size] = '\0';

  return RET_OK;
}

bool_t wstr_equal(wstr_t* str, wstr_t* other) {
  if (str == other) {
    return TRUE;
  }

  if (str == NULL || other == NULL) {
    return FALSE;
  }

  if (str->str == other->str) {
    return TRUE;
  }

  if (str->str == NULL || other->str == NULL) {
    return FALSE;
  }

  return wcscmp(str->str, other->str) == 0;
}

ret_t wstr_from_int(wstr_t* str, int32_t v) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return wstr_set_utf8(str, tk_itoa(buff, sizeof(buff), v));
}

ret_t wstr_from_float(wstr_t* str, double v) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return wstr_set_utf8(str, tk_ftoa(buff, sizeof(buff), v));
}

ret_t wstr_from_value(wstr_t* str, const value_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_STRING) {
    return wstr_set_utf8(str, value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    return wstr_set(str, value_wstr(v));
  } else if (v->type == VALUE_TYPE_FLOAT || v->type == VALUE_TYPE_FLOAT32 ||
             v->type == VALUE_TYPE_DOUBLE) {
    return wstr_from_float(str, value_float(v));
  } else {
    return wstr_from_int(str, value_int(v));
  }
}

ret_t wstr_to_int(wstr_t* str, int32_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  if (str->size > 0) {
    char buff[TK_NUM_MAX_LEN + 1] = {0};
    wchar_t wbuff[TK_NUM_MAX_LEN] = {0};
    uint32_t size = tk_min(str->size, TK_NUM_MAX_LEN) * sizeof(wchar_t);
    memcpy(wbuff, str->str, size);
    tk_utf8_from_utf16_ex(wbuff, ARRAY_SIZE(wbuff), buff, ARRAY_SIZE(buff));
    *v = tk_atoi(buff);
  } else {
    *v = 0;
  }

  return RET_OK;
}

ret_t wstr_to_float(wstr_t* str, double* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  if (str->size > 0) {
    char buff[TK_NUM_MAX_LEN + 1] = {0};
    wchar_t wbuff[TK_NUM_MAX_LEN] = {0};
    uint32_t size = tk_min(str->size, TK_NUM_MAX_LEN) * sizeof(wchar_t);

    memcpy(wbuff, str->str, size);
    if (tk_utf8_from_utf16_ex(wbuff, ARRAY_SIZE(wbuff), buff, ARRAY_SIZE(buff)) != NULL) {
      *v = tk_atof(buff);
    } else {
      *v = 0.0f;
      return RET_FAIL;
    }
  } else {
    *v = 0.0f;
  }

  return RET_OK;
}

static uint32_t double_fraction_nr(double v) {
  uint32_t nr = 0;
  double d = v - floor(v);
  if (tk_fequal(v, floor(v))) {
    return nr;
  }

  d = d * 10;
  nr++;
  while (d < 1 || d > floor(d)) {
    d = d * 10;
    nr++;
  }

  return nr;
}

static uint32_t wstr_fraction_nr(wstr_t* str) {
  uint32_t nr = 0;
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && str->str != NULL, 0);

  p = wcschr(str->str, '.');
  if (p != NULL) {
    nr = wcslen(p + 1);
  }

  return nr;
}

ret_t wstr_trim_float_zero(wstr_t* str) {
  wchar_t* p = NULL;

  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);
  if (wcschr(str->str, '.') == NULL) {
    return RET_OK;
  }

  p = str->str + str->size - 1;
  while (p > str->str) {
    if (*p != '0') {
      if (*p == '.') {
        p--;
      }
      break;
    }

    p--;
  }

  str->size = p - str->str + 1;
  str->str[str->size] = 0;

  return RET_OK;
}

ret_t wstr_to_fix(wstr_t* str, uint32_t fraction_nr) {
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  p = wcschr(str->str, '.');
  if (p) {
    p++;
    uint32_t nr = wcslen(p);
    if (nr > fraction_nr) {
      str->size = p - str->str + fraction_nr;
      str->str[str->size] = 0;
    }
  }

  return RET_OK;
}

ret_t wstr_add_float(wstr_t* str, double delta) {
  double v = 0;
  uint32_t fraction_nr1 = double_fraction_nr(delta);
  uint32_t fraction_nr2 = wstr_fraction_nr(str);
  uint32_t fraction_nr = tk_max(fraction_nr1, fraction_nr2);
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  wstr_to_float(str, &v);
  v = v + delta;
  wstr_from_float(str, v);

  return wstr_to_fix(str, fraction_nr);
}

ret_t wstr_reset(wstr_t* str) {
  return_value_if_fail(str != NULL, RET_OK);

  if (str->extendable) {
    TKMEM_FREE(str->str);
  }
  str->str = NULL;
  str->size = 0;
  str->capacity = 0;

  return RET_OK;
}

ret_t wstr_normalize_newline(wstr_t* str, wchar_t newline) {
  wchar_t* s = str->str;
  wchar_t* d = str->str;

  while (*s) {
    if (*s == '\r') {
      if (s[1] == '\n') {
        s++;
      }
      s++;
      *d++ = newline;
    } else if (*s == '\n') {
      s++;
      *d++ = newline;
    } else {
      *d++ = *s++;
    }
  }

  *d = '\0';
  str->size = d - str->str;

  return RET_OK;
}

uint32_t wstr_count_char(wstr_t* str, wchar_t c) {
  uint32_t i = 0;
  uint32_t n = 0;
  wchar_t* p = NULL;
  return_value_if_fail(str != NULL, 0);

  p = str->str;
  for (i = 0; i < str->size; i++) {
    if (p[i] == c) {
      n++;
    }
  }

  return n;
}
