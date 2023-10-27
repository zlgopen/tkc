﻿/**
 * File:   debugger_const.h
 * Author: AWTK Develop Team
 * Brief:  debugger constant
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_CONST_H
#define TK_DEBUGGER_CONST_H

#define DEBUGGER_VERSION 0x0100

#define STR_DEBUGGER_EVENT_PROP_LINE "line"
#define STR_DEBUGGER_EVENT_PROP_MESSAGE "message"
#define STR_DEBUGGER_EVENT_PROP_FILE_PATH "file_path"

#define DEBUGGER_IO_READ_TIMEOUT 1000000
#define DEBUGGER_IO_WRITE_TIMEOUT 500000

#ifndef DEBUGGER_TCP_PORT
#define DEBUGGER_TCP_PORT 6789
#endif /*DEBUGGER_TCP_PORT*/

#define DEBUGGER_START_LINE 0xffffffff

#define DEBUGGER_DEFAULT_CODE_ID "0"

#endif /*TK_DEBUGGER_CONST_H*/
