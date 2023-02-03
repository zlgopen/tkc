﻿/**
 * File:   json.h
 * Author: AWTK Develop Team
 * Brief:  json 
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_JSON_H
#define TK_CONF_JSON_H

#include "tkc/str.h"
#include "conf_io/conf_obj.h"

BEGIN_C_DECLS

/**
 * @class conf_json_t
 * @parent tk_object_t
 * @annotation ["fake"]
 * 
 * conf json对象。
 */

/**
 * @method conf_json_create 
 * 创建一个空的conf对象。 
 * @annotation ["constructor"]
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_create(void);

/**
 * @method conf_json_load 
 * 从指定URL加载JSON对象。 
 * @annotation ["constructor"]
 * 
 * @param {const char*} url 路径(通常是文件路径)。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_json_load(const char* url, bool_t create_if_not_exist);

/**
 * @method conf_json_save_as
 * 将doc对象保存到指定URL。
 * @annotation ["static"]
 * 
 * @param {tk_object_t*} obj doc对象。
 * @param {const char*} url 保存的位置。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_json_save_as(tk_object_t* obj, const char* url);

/**
 * @method conf_doc_load_json 
 * 
 * @annotation ["global"]
 * 
 * @param {const char*} data JSON数据。
 * @param {int32_t} size JSON数据长度。
 * 
 * @return {conf_doc_t*} 返回doc对象。
 */
conf_doc_t* conf_doc_load_json(const char* data, int32_t size);

/**
 * @method conf_doc_save_json 
 * 
 * @annotation ["global"]
 * 
 * @param {conf_doc_t*} doc doc对象。
 * @param {str_t*} str 保存结果。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_doc_save_json(conf_doc_t* doc, str_t* str);

END_C_DECLS

#endif /*TK_CONF_JSON_H*/
