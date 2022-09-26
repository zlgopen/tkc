/**
 * File:   fscript_bits.c
 * Author: AWTK Develop Team
 * Brief:  bits functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"

typedef enum _func_type_t {
  FUNC_LSHIFT = 1,
  FUNC_RSHIFT = 2,
  FUNC_BIT_CLEAR = 3,
} func_type_t;

static ret_t func_lshift(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);
  ret = value_lshift(value, result, n);

  return ret;
}

static ret_t func_rshift(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);
  ret = value_rshift(value, result, n);

  return ret;
}

static ret_t func_bit_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);
  
  return value_set_bit(value, result, n, TRUE);
}

static ret_t func_bit_clear(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  return value_set_bit(value, result, n, FALSE);
}

static ret_t func_bit_toggle(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  return value_toggle_bit(value, result, n);
}

static ret_t func_bit_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t n = 0;
  value_t* value = NULL;
  ret_t ret = RET_OK;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value = args->args;
  n = value_uint32(args->args + 1);

  return value_get_bit(value, result, n); 
}

static ret_t func_bit_and(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) & value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_or(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) | value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_nor(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  value_set_uint32(result, value_uint32(args->args) ^ value_uint32(args->args + 1));

  return RET_OK;
}

static ret_t func_bit_not(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  switch (args->args->type) {
    case VALUE_TYPE_INT8:
    case VALUE_TYPE_UINT8: {
      value_set_uint8(result, ~value_uint8(args->args));
      break;
    }
    case VALUE_TYPE_INT16:
    case VALUE_TYPE_UINT16: {
      value_set_uint16(result, ~value_uint16(args->args));
      break;
    }
    case VALUE_TYPE_INT32:
    case VALUE_TYPE_UINT32: {
      value_set_uint32(result, ~value_uint32(args->args));
      break;
    }
    case VALUE_TYPE_INT64:
    case VALUE_TYPE_UINT64: {
      value_set_uint64(result, ~value_uint64(args->args));
      break;
    }
    default: {
      value_set_uint32(result, ~value_uint32(args->args));
      break;
    }
  }

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_bits)
FACTORY_TABLE_ENTRY("&", func_bit_and)
FACTORY_TABLE_ENTRY("^", func_bit_nor)
FACTORY_TABLE_ENTRY("~", func_bit_not)
FACTORY_TABLE_ENTRY("|", func_bit_or)
FACTORY_TABLE_ENTRY("<<", func_lshift)
FACTORY_TABLE_ENTRY(">>", func_rshift)
FACTORY_TABLE_ENTRY("bit_get", func_bit_get)
FACTORY_TABLE_ENTRY("bit_set", func_bit_set)
FACTORY_TABLE_ENTRY("bit_clear", func_bit_clear)
FACTORY_TABLE_ENTRY("bit_toggle", func_bit_toggle)
FACTORY_TABLE_END()

ret_t fscript_bits_register(void) {
  return fscript_register_funcs(s_ext_bits);
}
