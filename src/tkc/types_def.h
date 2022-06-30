﻿/**
 * File:   types_def.h
 * Author: AWTK Develop Team
 * Brief:  basic types definitions.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TYPES_DEF_H
#define TYPES_DEF_H

#include <math.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#if defined(__MINGW32__) || defined(__MINGW64__)
#ifndef MINGW
#define MINGW 1
#endif
#endif

#if defined(HAS_AWTK_CONFIG)
#include "awtk_config.h"
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
#endif /*FRAGMENT_FRAME_BUFFER_SIZE*/
#endif /*HAS_AWTK_CONFIG*/

#if defined(WIN32) || defined(LINUX) || defined(MACOS) || defined(ANDROID) || defined(IOS)
#define WITH_SOCKET 1
#endif /*WIN32 || MACOS || LINUX || IOS || ANDROID*/

#ifdef WITH_SOCKET
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <winsock2.h>
typedef int socklen_t;
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif /*WIN32*/
#endif /*WITH_SOCKET*/
#ifdef __cplusplus
#define BEGIN_C_DECLS extern "C" {
#define END_C_DECLS }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#if defined(HAS_STDIO) || defined(AWTK_WEB)
#include <stdio.h>
#else
#define STBI_NO_STDIO
#endif /*HAS_STDIO*/

#ifndef TK_WEAK
#if defined(__CC_ARM) || \
    (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* ARM Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__) /* for IAR Compiler */
#define TK_WEAK __weak
#elif defined(__GNUC__) && !defined(__MINGW32__) && !defined(__MINGW64__) /* GNU GCC Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(__ADSPBLACKFIN__) /* for VisualDSP++ Compiler */
#define TK_WEAK __attribute__((weak))
#elif defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__) /* Windows */
#define TK_WEAK
#elif defined(__TI_COMPILER_VERSION__)
#define TK_WEAK
#else
#define TK_WEAK
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif /*TRUE*/

#ifndef FALSE
#define FALSE 0
#endif /*FALSE*/

#if defined(__GNUC__) && !defined(__cplusplus)
typedef _Bool bool_t;
#else
typedef uint8_t bool_t;
#endif

typedef int32_t xy_t;
typedef int32_t wh_t;
typedef uint16_t font_size_t;

/* 
 * 如果使用了 mingw 或者 32 位 pc 的 linux 出现 “conflicting declaration 'typedef float float_t'” 的错误提示的话，
 * 因为 awtk 的 float_t 类型为 float，所以请定义 __FLT_EVAL_METHOD__ 宏并且宏为 0 （#define __FLT_EVAL_METHOD__ 0 ）
 */
#if defined(WITH_DOUBLE_FLOAT)
typedef long double float_t;
#else
typedef float float_t;
#endif /*WITH_DOUBLE_FLOAT*/

#if !defined(ANDROID) && !defined(IOS) && !defined(LINUX) && !defined(MACOS) && !defined(WIN32)
#ifndef time_t
#define time_t uint64_t
#endif /*time_t*/
#endif

struct _value_t;
typedef struct _value_t value_t;

struct _tk_object_t;
typedef struct _tk_object_t tk_object_t;

#ifndef USE_TK_PREFIX
#define object_t tk_object_t
#endif /*USE_TK_PREFIX*/

/**
 * @enum ret_t
 * @annotation ["scriptable"]
 * 函数返回值常量定义。
 */
typedef enum _ret_t {
  /**
   * @const RET_OK
   * 成功。
   */
  RET_OK = 0,
  /**
   * @const RET_OOM
   * Out of memory。
   */
  RET_OOM,
  /**
   * @const RET_FAIL
   * 失败。
   */
  RET_FAIL,
  /**
   * @const RET_NOT_IMPL
   * 没有实现/不支持。
   */
  RET_NOT_IMPL,
  /**
   * @const RET_QUIT
   * 退出。通常用于主循环。
   */
  RET_QUIT,
  /**
   * @const RET_FOUND
   * 找到。
   */
  RET_FOUND,
  /**
   * @const RET_BUSY
   * 对象忙。
   */
  RET_BUSY,
  /**
   * @const RET_REMOVE
   * 移出。通常用于定时器。
   */
  RET_REMOVE,
  /**
   * @const RET_REPEAT
   * 重复。通常用于定时器。
   */
  RET_REPEAT,
  /**
   * @const RET_NOT_FOUND
   * 没找到。
   */
  RET_NOT_FOUND,
  /**
   * @const RET_DONE
   * 操作完成。
   */
  RET_DONE,
  /**
   * @const RET_STOP
   * 停止后续操作。
   */
  RET_STOP,
  /**
   * @const RET_SKIP
   * 跳过当前项。
   */
  RET_SKIP,
  /**
   * @const RET_CONTINUE
   * 继续后续操作。
   */
  RET_CONTINUE,
  /**
   * @const RET_OBJECT_CHANGED
   * 对象属性变化。
   */
  RET_OBJECT_CHANGED,
  /**
   * @const RET_ITEMS_CHANGED
   * 集合数目变化。
   */
  RET_ITEMS_CHANGED,
  /**
   * @const RET_BAD_PARAMS
   * 无效参数。
   */
  RET_BAD_PARAMS,
  /**
   * @const RET_TIMEOUT
   * 超时。
   */
  RET_TIMEOUT,
  /**
   * @const RET_CRC
   * CRC错误。
   */
  RET_CRC,
  /**
   * @const RET_IO
   * IO错误。
   */
  RET_IO,
  /**
   * @const RET_EOS
   * End of Stream
   */
  RET_EOS,
  /**
   * @const RET_NOT_MODIFIED
   * 没有改变。
   */
  RET_NOT_MODIFIED,
} ret_t;

#include "tkc/log.h"

#if defined(WIN32) || defined(__ARMCC_VERSION)
#define random rand
#define srandom srand
#endif /*WIN32||__ARMCC_VERSION*/

#if !defined(WIN32) && !defined(MAX_PATH)
#define MAX_PATH 255
#endif /*MAX_PATH*/

#if defined(WIN32)
#define TK_PATH_SEP '\\'
#else
#define TK_PATH_SEP '/'
#endif /*TK_PATH_SEP*/

#if defined(WIN32) && !defined(MINGW)
#ifndef snprintf
#define snprintf _snprintf
#endif /*snprintf*/

#define strcasecmp stricmp
#endif /*TK_PATH_SEP*/

#define log_if_fail(p)                                                     \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
  }

#if defined(NDEBUG) || defined(SYLIXOS)
#define ENSURE(p) (void)(p)
#define goto_error_if_fail(p) \
  if (!(p)) {                 \
    goto error;               \
  }

#define return_if_fail(p) \
  if (!(p)) {             \
    return;               \
  }

#define break_if_fail(p) \
  if (!(p)) {            \
    break;               \
  }

#define return_value_if_fail(p, value) \
  if (!(p)) {                          \
    return (value);                    \
  }
#else
#define ENSURE(p) assert(p)
#define goto_error_if_fail(p)                                              \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    goto error;                                                            \
  }

#define break_if_fail(p)                                                   \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    break;                                                                 \
  }

#define return_if_fail(p)                                                  \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    return;                                                                \
  }

#define return_value_if_fail(p, value)                                     \
  if (!(p)) {                                                              \
    log_warn("%s:%d condition(" #p ") failed!\n", __FUNCTION__, __LINE__); \
    return (value);                                                        \
  }

#endif

#define return_value_if_equal(p, value) \
  if ((p) == value) {                   \
    return (value);                     \
  }

#define tk_min(a, b) ((a) < (b) ? (a) : (b))
#define tk_abs(a) ((a) < (0) ? (-(a)) : (a))
#define tk_max(a, b) ((a) > (b) ? (a) : (b))
#define tk_roundi(a) (int32_t)(((a) >= 0) ? ((a) + 0.5f) : ((a)-0.5f))
#define tk_clamp(a, mn, mx) ((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))
#define tk_clampi(a, mn, mx) (int32_t)((a) < (mn) ? (mn) : ((a) > (mx) ? (mx) : (a)))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif /*ARRAY_SIZE*/

typedef void* (*tk_create_t)(void);
typedef ret_t (*tk_destroy_t)(void* data);
typedef ret_t (*tk_on_done_t)(void* data);
typedef ret_t (*tk_on_result_t)(void* ctx, const void* data);
typedef bool_t (*tk_is_valid_t)(void* data);

/*TRUE 保留，FALSE 忽略*/
typedef bool_t (*tk_filter_t)(void* ctx, const void* data);
typedef int (*tk_compare_t)(const void* a, const void* b);
typedef ret_t (*tk_visit_t)(void* ctx, const void* data);
typedef uint32_t (*tk_hash_t)(const void* data);
typedef ret_t (*tk_callback_t)(void* ctx);

/*TK_NAME_LEN+1 must aligned to 4*/
enum { TK_NAME_LEN = 31, TK_FUNC_NAME_LEN = 63 };

#ifdef WITH_CPPCHECK
#define tk_str_eq strcmp("abc", "123")
#define tk_str_ieq strcasecmp
#define tk_str_eq_with_len strncmp
#else
#define tk_str_eq(s1, s2)                                                           \
  (((s1) == NULL && (s2) == NULL) ||                                                \
   (((s1) != NULL) && ((s2) != NULL) && *((const char*)s1) == *((const char*)s2) && \
    strcmp((s1), (s2)) == 0))
#define tk_str_eq_with_len(s1, s2, len)                                            \
  (((s1) != NULL) && ((s2) != NULL) && *((const char*)s1) == *((const char*)s2) && \
   strncmp((s1), (s2), len) == 0)

#define tk_str_ieq(s1, s2)           \
  (((s1) == NULL && (s2) == NULL) || \
   (((s1) != NULL) && ((s2) != NULL) && strcasecmp((s1), (s2)) == 0))
#define tk_wstr_eq(s1, s2)                                                                \
  (((s1) == NULL && (s2) == NULL) ||                                                      \
   (((s1) != NULL) && ((s2) != NULL) && *((const wchar_t*)s1) == *((const wchar_t*)s2) && \
    wcscmp((s1), (s2)) == 0))
#endif /*WITH_CPPCHECK*/

#define tk_lfequal(f1, f2) (fabs((f1) - (f2)) < 0.0001)
#define tk_fequal(f1, f2) (fabs((f1) - (f2)) < 0.0000001)

#ifndef M_PI
#define M_PI 3.1415926f
#endif /*M_PI*/

#define TK_INVALID_ID 0u
#define TK_NUM_MAX_LEN 31u
#define TK_UINT32_MAX 0xffffffffu
#define TK_LOCALE_MAGIC "$locale$"
#define TK_OBJECT_PROP_NAME_MAX_LEN 64u

#define TK_D2R(d) (((d)*M_PI) / 180)
#define TK_R2D(r) (((r)*180) / M_PI)

#ifdef _MSC_VER
#define TK_CONST_DATA_ALIGN(v) __declspec(align(8)) v
#else
#define TK_CONST_DATA_ALIGN(v) v __attribute__((aligned(8)))
#endif /*_MSC_VER*/

typedef uint64_t (*tk_get_time_t)(void);
typedef uint64_t (*tk_get_time_ms_t)(void);
typedef void (*tk_sleep_ms_t)(uint32_t ms);

#if defined(WIN32) && !defined(NDEBUG)
#define TK_ENABLE_CONSOLE()                   \
  {                                           \
    AllocConsole();                           \
    FILE* fp = NULL;                          \
    freopen_s(&fp, "CONOUT$", "w+t", stdout); \
  }
#else
#define TK_ENABLE_CONSOLE()
#endif /*WIN32 && !NDEBUG*/

struct _event_source_t;
typedef struct _event_source_t event_source_t;

struct _event_source_manager_t;
typedef struct _event_source_manager_t event_source_manager_t;

#ifndef EAGAIN
#define EAGAIN 11
#endif /*EAGAIN*/

#define TK_SET_NULL(p) (p) = NULL
#define TK_ROUND_TO4(size) ((((size) + 3) >> 2) << 2)
#define TK_ROUND_TO8(size) ((((size) + 7) >> 3) << 3)
#define TK_ROUND_TO_MACH(size) ((sizeof(void*) == 4) ? TK_ROUND_TO4(size) : TK_ROUND_TO8(size))
#define TK_ROUND_TO(size, round_size) ((((size) + (round_size)-1) / (round_size)) * (round_size))

#define TK_SET_BIT(v, n) ((v) |= 1UL << (n))
#define TK_CLEAR_BIT(v, n) ((v) &= ~(1UL << (n)))
#define TK_TOGGLE_BIT(v, n) ((v) ^= (1UL << (n)))
#define TK_TEST_BIT(v, n) (((v) >> (n)) & 1U)

#ifndef TK_DEFAULT_WAIT_TIME
#define TK_DEFAULT_WAIT_TIME 16
#endif /*TK_DEFAULT_WAIT_TIME*/

/*from cairo/cairo/cairoint.h*/
#if _XOPEN_SOURCE >= 600 || defined(_ISOC99_SOURCE)
#define TK_ISFINITE(x) isfinite(x)
#else
#define TK_ISFINITE(x) ((x) * (x) >= 0.) /* check for NaNs */
#endif

#define tk_isspace(c) (((int)(c) < 128) && isspace(c))
#define tk_isdigit(c) (((int)(c) < 128) && isdigit(c))
#define tk_isxdigit(c) (((int)(c) < 128) && isxdigit(c))
#define tk_isprint(c) (((int)(c) < 128) && isprint(c))
#define tk_isalpha(c) (((int)(c) < 128) && isalpha(c))

#define STR_SCHEMA_TCP "tcp://"
#define STR_SCHEMA_UDP "udp://"
#define STR_SCHEMA_SERIAL "serial://"
#define STR_SCHEMA_FILE "file://"
#define STR_SCHEMA_FTP "ftp://"
#define STR_SCHEMA_HTTP "http://"
#define STR_SCHEMA_HTTPS "https://"

#define TK_STR_IS_EMPTY(s) (s == NULL || *s == '\0')
#define TK_STR_IS_NOT_EMPTY(s) ((s != NULL) && *s)

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif/*WIN32*/

#endif /*TYPES_DEF_H*/
