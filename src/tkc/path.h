﻿/**
 * File:   path.h
 * Author: AWTK Develop Team
 * Brief:  path
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
 * 2018-08-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PATH_H
#define TK_PATH_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class path_t
 * @annotation ["fake"]
 * 路径相关的工具函数。
 */

/**
 * @method path_basename
 *
 * 返回文件名。
 *
 * @param {const char*} path 路径。
 * @param {char*} result 用于返回文件名。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_basename(const char* path, char* result, int32_t size);

/**
 * @method path_basename_ex
 *
 * 返回文件名。
 *
 * @param {const char*} path 路径。
 * @param {bool_t} remove_ext_name 是否去掉扩展名。
 * @param {char*} result 用于返回文件名。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_basename_ex(const char* path, bool_t remove_ext_name, char* result, int32_t size);

/**
 * @method path_extname
 *
 * 返回文件扩展名。
 *
 * @param {const char*} path 路径。
 * @param {char*} result 用于返回文件扩展名。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_extname(const char* path, char* result, int32_t size);

/**
 * @method path_extname_is
 *
 * ```c
 * assert(path_extname_is("test.jpg", ".jpg"));
 * assert(path_extname_is("test.JPG", ".jpg"));
 * ```
 * 检查是否是指定的扩展名。
 *
 * @param {const char*} path 路径。
 * @param {const char*} extname 扩展名。
 *
 * @return {bool_t} 返回TRUE表示是，否则表示不是。
 */
bool_t path_extname_is(const char* path, const char* extname);

/**
 * @method path_dirname
 *
 * 返回目录。
 *
 * @param {const char*} path 路径。
 * @param {char*} result 用于返回目录。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_dirname(const char* path, char* result, int32_t size);

/**
 * @method path_normalize
 *
 * 规范路径字符形式。
 *
 * @param {const char*} path 路径。
 * @param {char*} result 用于返回规范后的路径。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_normalize(const char* path, char* result, int32_t size);

/**
 * @method path_abs
 *
 * 返回绝对路径。
 *
 * @param {const char*} path 路径。
 * @param {char*} result 用于返回绝对路径。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_abs(const char* path, char* result, int32_t size);

/**
 * @method path_is_abs
 *
 * 判断路径是否为绝对路径。
 *
 * @param {const char*} path 路径。
 *
 * @return {bool_t} 返回FALSE表示不是绝对路径，否则表示是绝对路径。
 */
bool_t path_is_abs(const char* path);

/**
 * @method path_build
 *
 * 构造路径。
 *
 *> 可变参数为字符串，以NULL参数结束。
 *
 * @param {char*} result 用于返回路径。
 * @param {int32_t} size 缓冲区大小。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_build(char* result, int32_t size, ...);

/**
 * @method path_replace_basename
 *
 * 替换文件名。
 *
 * @param {char*} result 用于返回结果。
 * @param {int32_t} size 缓冲区大小。
 * @param {const char*} filename 原始文件路径。
 * @param {const char*} basename 替换后的文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_replace_basename(char* result, int32_t size, const char* filename, const char* basename);

/**
 * @method path_replace_extname
 *
 * 替换文件扩展名。
 *
 * @param {char*} result 用于返回结果。
 * @param {int32_t} size 缓冲区大小。
 * @param {const char*} filename 原始文件路径。
 * @param {const char*} extname 替换后的文件扩展名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_replace_extname(char* result, int32_t size, const char* filename, const char* extname);

/**
 * @method path_cwd
 *
 * 获取当前所在目录。
 *
 * @param {char*} path 保存当前所在目录的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_cwd(char path[MAX_PATH + 1]);

/**
 * @method path_exe
 *
 * 获取可执行文件所在目录。
 *
 * @param {char*} path 保存可执行文件所在目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_exe(char path[MAX_PATH + 1]);

/**
 * @method path_app_root
 *
 * 获取app所在目录。
 *
 * @param {char*} path 保存app所在目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_app_root(char path[MAX_PATH + 1]);

/**
 * @method path_app_root_ex
 *
 * 获取app所在目录，并追加subpath到后面。
 *
 * @param {char*} path 保存app所在目录。
 * @param {const char*} subpath 子目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_app_root_ex(char path[MAX_PATH + 1], const char* subpath);

/**
 * @method path_exist
 *
 * 判断目录是否存在。
 *
 * @param {const char*} path 目录。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
bool_t path_exist(const char* path);

/**
 * @method path_remove_last_slash
 *
 * 去掉后面的/和\\字符。
 *
 * @param {char*} path 目录。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t path_remove_last_slash(char* path);

/**
 * @method path_prepend_app_root
 * 将前面路径加上app root。
 * @param {char*} full_path 用于返回完整路径。
 * @param {const char*} path 路径。
 *
 * @return {const char*} 返回完整路径。
 */
const char* path_prepend_app_root(char full_path[MAX_PATH + 1], const char* path);

END_C_DECLS

#endif /*TK_PATH_H*/
