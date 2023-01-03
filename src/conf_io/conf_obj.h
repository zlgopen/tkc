﻿
/**
 * File:   conf_obj.h
 * Author: AWTK Develop Team
 * Brief:  conf obj
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
 * 2020-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CONF_OBJ_H
#define TK_CONF_OBJ_H

#include "tkc/object.h"
#include "tkc/data_reader.h"
#include "tkc/data_writer.h"
#include "conf_io/conf_node.h"

BEGIN_C_DECLS

typedef ret_t (*conf_doc_save_t)(conf_doc_t* doc, data_writer_t* writer);
typedef conf_doc_t* (*conf_doc_load_t)(data_reader_t* reader);

/**
 * @class conf_obj_t
 * @parent tk_object_t
 * @annotation ["fake"]
 * 
 * conf对象。
 */

/**
 * @method conf_obj_create 
 * 
 * @annotation ["constructor"]
 * 
 * @param {conf_doc_save_t} save 保存函数。
 * @param {conf_doc_load_t} load 加载函数。
 * @param {const char*} url 路径。
 * @param {bool_t} create_if_not_exist 如果不存在是否创建。 
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_obj_create(conf_doc_save_t save, conf_doc_load_t load, const char* url,
                             bool_t create_if_not_exist);

/**
 * @method conf_obj_create_sub_object 
 * 根据路径创建一个子对象。 
 * @annotation ["constructor"]
 * 
 * @param {tk_object_t*} conf 配置对象。
 * @param {const char*} path 路径。
 * 
 * @return {tk_object_t*} 返回配置对象。
 */
tk_object_t* conf_obj_create_sub_object(tk_object_t* conf, const char* path);

/**
 * @method conf_obj_get_doc 
 * 
 * 获取doc对象，用于更高级的操作。
 * 
 * @param {tk_object_t*} conf conf对象。
 * 
 * @return {conf_doc_t*} 返回doc对象。
 * 
 */
conf_doc_t* conf_obj_get_doc(tk_object_t* conf);

/**
 * @method conf_obj_save 
 * 
 * 保存文档。
 * 
 * @param {tk_object_t*} conf conf对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_obj_save(tk_object_t* conf);

/**
 * @method conf_obj_set_readonly
 * 
 * 设置为只读模式。
 * 
 * @param {tk_object_t*} conf conf对象。
 * @param {bool_t} readonly 只读模式。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败
 */
ret_t conf_obj_set_readonly(tk_object_t* conf, bool_t readonly);

#define CONF_OBJ_PROP_URL "__URL__"
#define CONF_OBJ_PROP_DEFAULT_URL "__DEFAULT_URL__"

END_C_DECLS

#endif /*TK_CONF_OBJ_H*/
