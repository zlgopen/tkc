﻿/**
 * File:   str.h
 * Author: AWTK Develop Team
 * Brief:  string
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
 * 2018-04-30 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#ifndef TK_STR_H
#define TK_STR_H

#include "tkc/value.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class str_t
 * 可变长度的UTF8字符串。
 *
 * 示例：
 *
 * ```c
 *  str_t s;
 *  str_init(&s, 0);
 *
 *  str_append(&s, "abc");
 *  str_append(&s, "123");
 *  log_debug("%s\n", s.str);
 *
 *  str_reset(&s);
 * ```
 *
 * > 先调str\_init进行初始化，最后调用str\_reset释放内存。
 *
 */
typedef struct _str_t {
  /**
   * @property {uint32_t} size
   * @annotation ["readable"]
   * 长度。
   */
  uint32_t size;
  /**
   * @property {uint32_t} capacity
   * @annotation ["readable"]
   * 容量。
   */
  uint32_t capacity;
  /**
   * @property {char*} str
   * @annotation ["readable"]
   * 字符串。
   */
  char* str;
  /*private*/
  bool_t extendable;
} str_t;

/**
 * @method str_create
 * 创建str对象。
 * @annotation ["constructor"]
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {str_t*} str对象。
 */
str_t* str_create(uint32_t capacity);

/**
 * @method str_destroy
 * 销毁str对象
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_destroy(str_t* str);

/**
 * @method str_init
 * 初始化字符串对象。
 * @annotation ["constructor"]
 * @param {str_t*} str str对象。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {str_t*} str对象本身。
 */
str_t* str_init(str_t* str, uint32_t capacity);

/**
 * @method str_attach
 * 通过附加到一个buff来初始化str。 
 * >可以避免str动态分配内存，同时也不会自动扩展内存，使用完成后无需调用str_reset。
 *```c
 * str_t s;
 * char buff[32];
 * str_attach(&s, buff, ARRAY_SIZE(buff));
 * str_set(&s, "abc");
 * str_append(&s, "123");
 *```
 * @annotation ["constructor"]
 * @param {str_t*} str str对象。
 * @param {char*} buff 缓冲区。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {str_t*} str对象本身。
 */
str_t* str_attach(str_t* str, char* buff, uint32_t capacity);

/**
 * @method str_attach_with_size
 * 通过附加到一个buff来初始化str。 
 * >可以避免str动态分配内存，同时也不会自动扩展内存，使用完成后无需调用str_reset。
 *```c
 * str_t s;
 * char buff[32];
 * strcpy(buff, "a");
 * str_attach_with_size(&s, buff, 1, ARRAY_SIZE(buff));
 * str_set(&s, "abc");
 * str_append(&s, "123");
 *```
 * @annotation ["constructor"]
 * @param {str_t*} str str对象。
 * @param {char*} buff 缓冲区。
 * @param {uint32_t} size 初始长度。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {str_t*} str对象本身。
 */
str_t* str_attach_with_size(str_t* str, char* buff, uint32_t size, uint32_t capacity);

/**
 * @method str_extend
 * 扩展字符串到指定的容量。
 * @param {str_t*} str str对象。
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_extend(str_t* str, uint32_t capacity);

/**
 * @method str_eq
 * 判断两个字符串是否相等。
 * @param {str_t*} str str对象。
 * @param {const char*} text 待比较的字符串。
 *
 * @return {bool_t} 返回是否相等。
 */
bool_t str_eq(str_t* str, const char* text);

/**
 * @method str_set
 * 设置字符串。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要设置的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_set(str_t* str, const char* text);

/**
 * @method str_clear
 * 清除字符串内容。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_clear(str_t* str);

/**
 * @method str_set_with_len
 * 设置字符串。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要设置的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_set_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_append
 * 追加字符串。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append(str_t* str, const char* text);

/**
 * @method str_append_more
 * 追加多个字符串。以NULL结束。
 *
 * 示例：
 *
 * ```c
 *  str_t s;
 *  str_init(&s, 0);
 *
 *  str_append_more(&s, "abc", "123", NULL);
 *  log_debug("%s\n", s.str);
 *
 *  str_reset(&s);
 * ```
 * @param {str_t*} str str对象。
 * @param {const char*} text 要追加的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_more(str_t* str, const char* text, ...);

/**
 * @method str_append_with_len
 * 追加字符串。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要追加的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_insert
 * 插入子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {const char*} text 要插入的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_insert(str_t* str, uint32_t offset, const char* text);

/**
 * @method str_insert_with_len
 * 插入子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {const char*} text 要插入的字符串。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_insert_with_len(str_t* str, uint32_t offset, const char* text, uint32_t len);

/**
 * @method str_remove
 * 删除子字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} offset 偏移量。
 * @param {uint32_t} len 长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_remove(str_t* str, uint32_t offset, uint32_t len);

/**
 * @method str_append_char
 * 追加一个字符。
 * @param {str_t*} str str对象。
 * @param {char} c 要追加的字符。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_char(str_t* str, char c);

/**
 * @method str_append_n_chars
 * 同一个字符追加n次。
 * @param {str_t*} str str对象。
 * @param {char} c 要追加的字符。
 * @param {uint32_t} n 字符的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_n_chars(str_t* str, char c, uint32_t n);

/**
 * @method str_append_int
 * 追加一个整数。
 * @param {str_t*} str str对象。
 * @param {int32_t} value 要追加的整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_int(str_t* str, int32_t value);

/**
 * @method str_append_int64
 * 追加一个int64整数。
 * @param {str_t*} str str对象。
 * @param {int64_t} value 要追加的整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_int64(str_t* str, int64_t value);

/**
 * @method str_append_uint64
 * 追加一个uint64整数。
 * @param {str_t*} str str对象。
 * @param {uint64_t} value 要追加的整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_uint64(str_t* str, uint64_t value);

/**
 * @method str_append_double
 * 追加一个浮点数。
 * @param {str_t*} str str对象。
 * @param {const char*} format 格式。
 * @param {double} value 要追加的浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_double(str_t* str, const char* format, double value);

/**
 * @method str_append_json_str
 * 追加一个字符串，字符串前后加英文双引号，并按JSON规则转义特殊字符。
 * @param {str_t*} str str对象。
 * @param {const char*} json_str 待追加的字符串。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_str(str_t* str, const char* json_str);

/**
 * @method str_append_c_str
 * 追加一个字符串，字符串前后加英文双引号，并按C语言规则转义特殊字符。
 * @param {str_t*} str str对象。
 * @param {const char*} c_str 待追加的字符串。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_c_str(str_t* str, const char* c_str);

/**
 * @method str_append_json_int_pair
 * 追加int格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {int32_t} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_int_pair(str_t* str, const char* key, int32_t value);

/**
 * @method str_append_json_str_pair
 * 追加字符串格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {const char*} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_str_pair(str_t* str, const char* key, const char* value);

/**
 * @method str_append_json_double_pair
 * 追加doube格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {double} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_double_pair(str_t* str, const char* key, double value);

/**
 * @method str_append_json_bool_pair
 * 追加bool格式的json键值对。
 * @param {str_t*} str str对象。
 * @param {const char*} key 键。 
 * @param {bool_t} value 值。 
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_json_bool_pair(str_t* str, const char* key, bool_t value);

/**
 * @method str_pop
 * 删除最后一个字符。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_pop(str_t* str);

/**
 * @method str_unescape
 * 对字符串进行反转义。如：把"\n"转换成'\n'。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_unescape(str_t* str);

/**
 * @method str_append_escape
 * 对字符串s进行转义，并追加到str对象。
 * @param {str_t*} str str对象。
 * @param {const char*} s 字符串。
 * @param {uint32_t} size 字符串s的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_escape(str_t* str, const char* s, uint32_t size);

/**
 * @method str_append_unescape
 * 对字符串s进行反转义，并追加到str对象。
 * @param {str_t*} str str对象。
 * @param {const char*} s 字符串。
 * @param {uint32_t} size 字符串s的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_unescape(str_t* str, const char* s, uint32_t size);

/**
 * @method str_decode_xml_entity
 * 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要解码的XML文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_decode_xml_entity(str_t* str, const char* text);

/**
 * @method str_decode_xml_entity_with_len
 * 对XML基本的entity进行解码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要解码的XML文本。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_decode_xml_entity_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_encode_xml_entity
 * 对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要编码的XML文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_encode_xml_entity(str_t* str, const char* text);

/**
 * @method str_encode_xml_entity_with_len
 * 对XML基本的entity进行编码，目前仅支持&lt;&gt;&quota;&amp;。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要编码的XML文本。
 * @param {uint32_t} len 字符串长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_encode_xml_entity_with_len(str_t* str, const char* text, uint32_t len);

/**
 * @method str_from_int
 * 用整数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {int32_t} value 整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_int(str_t* str, int32_t value);

/**
 * @method str_from_uint32
 * 用整数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} value 整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_uint32(str_t* str, uint32_t value);

/**
 * @method str_from_int64
 * 用整数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {int64_t} value 整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_int64(str_t* str, int64_t value);

/**
 * @method str_from_uint64
 * 用整数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {uint64_t} value 整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_uint64(str_t* str, uint64_t value);

/**
 * @method str_from_float
 * 用浮点数初始化字符串。
 * @param {str_t*} str str对象。
 * @param {double} value 浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_float(str_t* str, double value);

/**
 * @method str_from_value
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {const value_t*} value value。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_value(str_t* str, const value_t* value);

/**
 * @method str_from_wstr
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {const wchar_t*} wstr Unicode字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_wstr(str_t* str, const wchar_t* wstr);

/**
 * @method str_from_wstr_with_len
 * 用value初始化字符串。
 * @param {str_t*} str str对象。
 * @param {const wchar_t*} wstr Unicode字符串
 * @param {uint32_t} len Unicode字符串的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_from_wstr_with_len(str_t* str, const wchar_t* wstr, uint32_t len);

/**
 * @method str_to_int
 * 将字符串转成整数。
 * @param {str_t*} str str对象。
 * @param {int32_t*} value 用于返回整数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_int(str_t* str, int32_t* value);

/**
 * @method str_to_float
 * 将字符串转成浮点数。
 * @param {str_t*} str str对象。
 * @param {double*} value 用于返回浮点数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_float(str_t* str, double* value);

/**
 * @method str_encode_hex
 * 把二进制的数据编码成16进制格式的字符串。
 * @param {str_t*} str str对象。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} size 数据长度。
 * @param {const char*} format 格式(如:"%02x" 表示生成小写)
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_encode_hex(str_t* str, const uint8_t* data, uint32_t size, const char* format);

/**
 * @method str_decode_hex
 * 把16进制格式的字符串解码成字符串。
 * @param {str_t*} str str对象。
 * @param {uint8_t*} data 数据缓存区(返回)。
 * @param {uint32_t} size 数据最大长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_decode_hex(str_t* str, uint8_t* data, uint32_t size);

/**
 * @method str_end_with
 * 判断字符串是否以指定的子串结尾。
 * @param {str_t*} str str对象。
 * @param {const char*} text 子字符串。
 *
 * @return {bool_t} 返回是否以指定的子串结尾。
 */
bool_t str_end_with(str_t* str, const char* text);

/**
 * @method str_start_with
 * 判断字符串是否以指定的子串开头。
 * @param {str_t*} str str对象。
 * @param {const char*} text 子字符串。
 *
 * @return {bool_t} 返回是否以指定的子串开头。
 */
bool_t str_start_with(str_t* str, const char* text);

/**
 * @method str_trim
 * 去除首尾指定的字符。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim(str_t* str, const char* text);

/**
 * @method str_trim_left
 * 去除首部指定的字符。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim_left(str_t* str, const char* text);

/**
 * @method str_trim_right
 * 去除尾部指定的字符。
 * @param {str_t*} str str对象。
 * @param {const char*} text 要去除的字符集合。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_trim_right(str_t* str, const char* text);

/**
 * @method str_replace
 * 字符串替换。
 * @param {str_t*} str str对象。
 * @param {const char*} text 待替换的子串。
 * @param {const char*} new_text 将替换成的子串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_replace(str_t* str, const char* text, const char* new_text);

/**
 * @method str_to_lower
 * 将字符串转成小写。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_lower(str_t* str);

/**
 * @method str_to_upper
 * 将字符串转成大写。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_to_upper(str_t* str);

/**
 * @method str_expand_vars
 * 将字符串中的变量展开为obj中对应的属性值。
 *
 * 变量的格式为${xxx}：
 *
 * * xxx为变量名时，${xxx}被展开为obj的属性xxx的值。
 * * xxx为表达式时，${xxx}被展开为表达式的值，表达式中可以用变量，$为变量的前缀，如${$x+$y}。
 * * xxx为变量名时，而不存在obj的属性时，${xxx}被移出。
 *
 * @param {str_t*} str str对象。
 * @param {const char*} src 字符串。
 * @param {const tk_object_t*} obj obj对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_expand_vars(str_t* str, const char* src, const tk_object_t* obj);

/**
 * @method str_common_prefix
 * 计算str和other的共同前缀，并设置到str中。
 * @param {str_t*} str str对象。
 * @param {const char*} other 另外一个字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_common_prefix(str_t* str, const char* other);

/**
 * @method str_reset
 * 重置字符串为空。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_reset(str_t* str);

/**
 * @method str_reverse
 * 前后颠倒字符串。
 * @param {str_t*} str str对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_reverse(str_t* str);

/**
 * @method str_count
 * 统计字串出现的次数。
 * @param {str_t*} str str对象。
 * @param {const char*} substr 字串。
 *
 * @return {uint32_t} 返回字符串出现的次数。
 */
uint32_t str_count(str_t* str, const char* substr);

/**
 * @method str_format
 * 通过格式设置字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} size format生成的字符串的最大长度(用于预先分配内存)。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_format(str_t* str, uint32_t size, const char* format, ...);

/**
 * @method str_append_format
 * 通过格式追加字符串。
 * @param {str_t*} str str对象。
 * @param {uint32_t} size format生成的字符串的最大长度(用于预先分配内存)。
 * @param {const char*} format 格式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t str_append_format(str_t* str, uint32_t size, const char* format, ...);

END_C_DECLS

#endif /*TK_STR_H*/
