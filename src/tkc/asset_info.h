﻿/**
 * File:   asset_info.h
 * Author: AWTK Develop Team
 * Brief:  asset info
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
 * 2018-03-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASSET_INFO_H
#define TK_ASSET_INFO_H

#ifdef LOAD_ASSET_WITH_MMAP
#include "tkc/mmap.h"
#else
#include "tkc/types_def.h"
#endif /*LOAD_ASSET_WITH_MMAP*/

BEGIN_C_DECLS

/**
 * @enum asset_type_t
 * @prefix ASSET_TYPE_
 * @annotation ["scriptable"]
 * 资源类型常量定义。
 */
typedef enum _asset_type_t {
  /**
   * @const ASSET_TYPE_NONE
   * 无效资源。
   */
  ASSET_TYPE_NONE,

  /**
   * @const ASSET_TYPE_FONT
   * 字体资源。
   */
  ASSET_TYPE_FONT,

  /**
   * @const ASSET_TYPE_IMAGE
   * 图片资源。
   */
  ASSET_TYPE_IMAGE,

  /**
   * @const ASSET_TYPE_STYLE
   * 窗体样式资源。
   */
  ASSET_TYPE_STYLE,

  /**
   * @const ASSET_TYPE_UI
   * UI数据资源。
   */
  ASSET_TYPE_UI,

  /**
   * @const ASSET_TYPE_XML
   * XML数据资源。
   */
  ASSET_TYPE_XML,

  /**
   * @const ASSET_TYPE_STRINGS
   * 字符串数据资源。
   */
  ASSET_TYPE_STRINGS,

  /**
   * @const ASSET_TYPE_SCRIPT
   * JS等脚本资源。
   */
  ASSET_TYPE_SCRIPT,

  /**
   * @const ASSET_TYPE_FLOW
   * 流图资源。
   */
  ASSET_TYPE_FLOW,
  /**
   * @const ASSET_TYPE_DATA
   * 其它数据资源。
   */
  ASSET_TYPE_DATA
} asset_type_t;

/**
 * @enum asset_font_type_t
 * @prefix ASSET_TYPE_FONT_
 * 字体资源类型定义。
 */
typedef enum _asset_font_type_t {
  /**
   * @const ASSET_TYPE_FONT_NONE
   * 无效字体。
   */
  ASSET_TYPE_FONT_NONE,
  /**
   * @const ASSET_TYPE_FONT_TTF
   * TTF字体。
   */
  ASSET_TYPE_FONT_TTF,
  /**
   * @const ASSET_TYPE_FONT_BMP
   * 位图字体。
   */
  ASSET_TYPE_FONT_BMP
} asset_font_type_t;

/**
 * @enum asset_data_type_t
 * @prefix ASSET_TYPE_DATA_
 * 数据资源类型定义。
 */
typedef enum _asset_data_type_t {
  /**
   * @const ASSET_TYPE_DATA_NONE
   * 未知数据类型。
   */
  ASSET_TYPE_DATA_NONE,
  /**
   * @const ASSET_TYPE_DATA_TEXT
   * 文本数据类型。
   */
  ASSET_TYPE_DATA_TEXT,
  /**
   * @const ASSET_TYPE_DATA_BIN
   * 二进制数据类型。
   */
  ASSET_TYPE_DATA_BIN,
  /**
   * @const ASSET_TYPE_DATA_JSON
   * JSON数据类型。
   */
  ASSET_TYPE_DATA_JSON,
  /**
   * @const ASSET_TYPE_DATA_DAT
   * 通用数据类型。
   */
  ASSET_TYPE_DATA_DAT
} asset_data_type_t;

/**
 * @enum asset_script_type_t
 * @prefix ASSET_TYPE_SCRIPT_
 * 脚本资源类型定义。
 */
typedef enum _asset_script_type_t {
  /**
   * @const ASSET_TYPE_SCRIPT_NONE
   * 未知脚本类型。
   */
  ASSET_TYPE_SCRIPT_NONE,
  /**
   * @const ASSET_TYPE_SCRIPT_JS
   * JS脚本类型。
   */
  ASSET_TYPE_SCRIPT_JS,
  /**
   * @const ASSET_TYPE_SCRIPT_LUA
   * LUA脚本类型。
   */
  ASSET_TYPE_SCRIPT_LUA,
  /**
   * @const ASSET_TYPE_SCRIPT_PYTHON
   * Python脚本类型。
   */
  ASSET_TYPE_SCRIPT_PYTHON
} asset_script_type_t;

/**
 * @enum asset_image_type_t
 * @prefix ASSET_TYPE_IMAGE_
 * 图片资源类型定义。
 */
typedef enum _asset_image_type_t {
  /**
   * @const ASSET_TYPE_IMAGE_NONE
   * 未知图片类型。
   */
  ASSET_TYPE_IMAGE_NONE,
  /**
   * @const ASSET_TYPE_IMAGE_RAW
   * Raw图片类型。
   */
  ASSET_TYPE_IMAGE_RAW,
  /**
   * @const ASSET_TYPE_IMAGE_BMP
   * 位图图片类型。
   */
  ASSET_TYPE_IMAGE_BMP,
  /**
   * @const ASSET_TYPE_IMAGE_PNG
   * PNG图片类型。
   */
  ASSET_TYPE_IMAGE_PNG,
  /**
   * @const ASSET_TYPE_IMAGE_JPG
   * JPG图片类型。
   */
  ASSET_TYPE_IMAGE_JPG,
  /**
   * @const ASSET_TYPE_IMAGE_BSVG
   * BSVG图片类型。
   */
  ASSET_TYPE_IMAGE_BSVG,
  /**
   * @const ASSET_TYPE_IMAGE_GIF
   * GIF图片类型。
   */
  ASSET_TYPE_IMAGE_GIF,
  /**
   * @const ASSET_TYPE_IMAGE_WEBP
   * WEBP图片类型。
   */
  ASSET_TYPE_IMAGE_WEBP,
  /**
   * @const ASSET_TYPE_IMAGE_LZ4
   * LZ4压缩的图片类型。
   */
  ASSET_TYPE_IMAGE_LZ4,
  /**
   * @const ASSET_TYPE_IMAGE_OTHER
   * 其它图片类型。
   */
  ASSET_TYPE_IMAGE_OTHER
} asset_image_type_t;

/**
 * @enum asset_ui_type_t
 * @prefix ASSET_TYPE_UI_
 * UI资源类型定义。
 */
typedef enum _asset_ui_type_t {
  /**
   * @const ASSET_TYPE_UI_NONE
   * 无效UI类型。
   */
  ASSET_TYPE_UI_NONE,
  /**
   * @const ASSET_TYPE_UI_BIN
   * 二进制的UI类型。
   */
  ASSET_TYPE_UI_BIN,
  /**
   * @const ASSET_TYPE_UI_XML
   * XML格式的UI类型。
   */
  ASSET_TYPE_UI_XML
} asset_ui_type_t;

/**
 * @class preload_res_t
 * 预加载资源的描述信息。
 */
typedef struct _preload_res_t {
  asset_type_t type;
  const char* name;
} preload_res_t;

/**
 * @enum asset_info_flag_t
 * @prefix ASSET_INFO_FLAG_
 * 资源标志常量定义。
 */
typedef enum _asset_info_flag_t {
  /**
   * @const ASSET_INFO_FLAG_IN_ROM
   * 资源在ROM中。
   */
  ASSET_INFO_FLAG_IN_ROM = 1,
  /**
   * @const ASSET_INFO_FLAG_FULL_NAME
   * 使用长名字。
   */
  ASSET_INFO_FLAG_FULL_NAME = 1 << 1,
} asset_info_flag_t;

/**
 * @class asset_info_t
 * @annotation ["scriptable"]
 * 单个资源的描述信息。
 */
#pragma pack(push, 1)
typedef struct _asset_info_t {
  /**
   * @property {uint16_t} type
   * @annotation ["readable","scriptable"]
   * 类型。
   */
  uint16_t type;
  /**
   * @property {uint8_t} subtype
   * @annotation ["readable", "scriptable"]
   * 子类型。
   */
  uint8_t subtype;
  /**
   * @property {uint8_t} flags
   * @annotation ["readable", "scriptable"]
   * 资源标志。
   */
  uint8_t flags;
  /**
   * @property {uint32_t} size
   * @annotation ["readable","scriptable"]
   * 大小。
   */
  uint32_t size;
  /**
   * @property {uint32_t} refcount
   * @annotation ["readable","scriptable"]
   * 引用计数。
   * is\_in\_rom == FALSE时才有效。
   */
  uint32_t refcount;
  /* internal */
  union {
    char small_name[TK_NAME_LEN + 1];
    char* full_name;
  } name;

#ifdef LOAD_ASSET_WITH_MMAP
  uint8_t* data;
  mmap_t* map;
#else
  uint8_t data[4];
#endif /*LOAD_ASSET_WITH_MMAP*/
} asset_info_t;
#pragma pack(pop)

/**
 * @method asset_info_create
 * 创建asset_info对象。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["constructor"]
 * @param {uint16_t} type 资源的类型。 
 * @param {uint16_t} subtype  资源的子类型。
 * @param {const char*} name 资源的名称。
 * @param {int32_t} size  资源的数据长度(用于分配空间)。
 *
 * @return {asset_info_t*} asset_info对象。
 */
asset_info_t* asset_info_create(uint16_t type, uint16_t subtype, const char* name, int32_t size);

/**
 * @method asset_info_destroy
 *
 * 销毁asset_info对象。
 *
 * @annotation ["deconstructor"]
 * @param {asset_info_t*} info asset_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t asset_info_destroy(asset_info_t* info);

/**
 * @method asset_info_unref
 *
 * 减少asset_info对象的引用计数。
 *
 * @param {asset_info_t*} info asset_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t asset_info_unref(asset_info_t* info);

/**
 * @method asset_info_ref
 *
 * 增加asset_info对象的引用计数。
 *
 * @param {asset_info_t*} info asset_info对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t asset_info_ref(asset_info_t* info);

/**
 * @method asset_info_get_type
 *
 * 获取类型。
 * @annotation ["scriptable"]
 *
 * @param {asset_info_t*} info asset_info对象。
 *
 * @return {uint16_t} 返回类型。
 */
uint16_t asset_info_get_type(asset_info_t* info);

/**
 * @method asset_info_get_name
 *
 * 获取名称。
 * @annotation ["scriptable"]
 *
 * @param {const asset_info_t*} info asset_info对象。
 *
 * @return {const char*} 返回名称。
 */
const char* asset_info_get_name(const asset_info_t* info);

/**
 * @method asset_info_get_formatted_name
 *
 * 把资源名字格式化为符合标准长度的字符串。
 *
 * @param {const char*} name 未格式化名字。
 *
 * @return {const char*} 返回格式化后的名字。
 */
const char* asset_info_get_formatted_name(const char* name);

/**
 * @method asset_info_is_in_rom
 *
 * 资源是否在ROM中。
 * @annotation ["scriptable"]
 *
 * @param {const asset_info_t*} info asset_info对象。
 *
 * @return {bool_t} 返回 TRUE 为在 ROM 中，返回 FALSE 则不在。
 */
bool_t asset_info_is_in_rom(const asset_info_t* info);

/**
 * @method asset_info_set_is_in_rom
 *
 * 设置资源是否在ROM中的标记位。
 * @annotation ["scriptable"]
 *
 * @param {asset_info_t*} info asset_info对象。
 * @param {bool_t} is_in_rom 资源是否在ROM中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t asset_info_set_is_in_rom(asset_info_t* info, bool_t is_in_rom);

/* internal */
ret_t asset_info_set_name(asset_info_t* info, const char* name, bool_t is_alloc);

END_C_DECLS

#endif /*TK_ASSET_INFO_H*/
