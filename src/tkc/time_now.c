﻿/**
 * File:   time.h
 * Author: AWTK Develop Team
 * Brief:  time
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
 * 2018-04-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/time_now.h"
#include "tkc/platform.h"

#ifndef HAS_GET_TIME_US64
uint64_t get_time_us64() {
  return get_time_ms64() * 1000;
}
#endif /*HAS_GET_TIME_US64*/

uint64_t time_now_us(void) {
  return get_time_us64();
}

uint64_t time_now_ms(void) {
  return get_time_ms64();
}

uint64_t time_now_s(void) {
  return get_time_ms64() / 1000;
}
