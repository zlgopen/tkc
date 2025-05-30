﻿/**
 * File:   ubjson_parser.c
 * Author: AWTK Develop Team
 * Brief:  ubjson parser
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "tkc/object_array.h"
#include "ubjson/ubjson_parser.h"

#define MAX_LEVEL 10

typedef struct _ubjson_parser_t {
  str_t temp;
  rbuffer_t rb;
  ubjson_reader_t reader;

  tk_object_t* obj;
  tk_object_t* root;

  void* ctx;
  ubjson_on_key_value_t on_key_value;

  bool_t error;
  uint32_t level;
  tk_object_t* stack[MAX_LEVEL + 1];

  uint32_t optimized_type;
} ubjson_parser_t;

static ret_t ubjson_do_parse_object(ubjson_parser_t* parser);
static ret_t ubjson_do_parse_array(ubjson_parser_t* parser);
static ret_t ubjson_do_parse_key_value(ubjson_parser_t* parser, int32_t len);

static ret_t ubjson_parser_on_key_value(ubjson_parser_t* parser, const char* key, value_t* v) {
  ret_t ret = parser->on_key_value(parser->ctx, key, v);

  if (ret != RET_OK) {
    parser->error = TRUE;
  }

  return ret;
}

static ret_t rbuffer_read_binary_if_has_more(rbuffer_t* rbuffer, void* data, uint32_t size) {
  if (rbuffer_has_more(rbuffer)) {
    return rbuffer_read_binary(rbuffer, data, size);
  }

  return RET_DONE;
}

static ret_t ubjson_parser_push(ubjson_parser_t* parser, tk_object_t* obj) {
  return_value_if_fail(parser->level < MAX_LEVEL, RET_BAD_PARAMS);

  parser->obj = obj;
  parser->stack[parser->level] = obj;
  parser->level++;

  return RET_OK;
}

static ret_t ubjson_parser_pop(ubjson_parser_t* parser) {
  return_value_if_fail(parser->level > 0, RET_BAD_PARAMS);

  parser->level--;
  parser->stack[parser->level] = NULL;

  if (parser->level > 0) {
    parser->obj = parser->stack[parser->level - 1];
  } else {
    parser->obj = NULL;
  }

  return RET_OK;
}

static ubjson_parser_t* ubjson_parser_init(ubjson_parser_t* parser, void* data, uint32_t size,
                                           ubjson_on_key_value_t on_key_value, void* ctx) {
  str_t* temp = &(parser->temp);
  rbuffer_t* rb = &(parser->rb);
  ubjson_reader_t* reader = &(parser->reader);

  memset(parser, 0x00, sizeof(ubjson_parser_t));

  parser->ctx = ctx;
  parser->on_key_value = on_key_value;

  str_init(temp, 64);
  rbuffer_init(rb, data, size);
  ubjson_reader_init(reader, (ubjson_read_callback_t)rbuffer_read_binary_if_has_more, rb);

  return parser;
}

static ret_t ubjson_parser_deinit(ubjson_parser_t* parser) {
  str_t* temp = &(parser->temp);
  ubjson_reader_t* reader = &(parser->reader);

  str_reset(temp);
  ubjson_reader_reset(reader);

  return RET_OK;
}

static ret_t ubjson_parser_read(ubjson_parser_t* parser, value_t* v) {
  ubjson_reader_t* reader = &(parser->reader);
  return_value_if_fail(parser->error == FALSE, RET_FAIL);

  return ubjson_reader_read(reader, v);
}

static ret_t ubjson_on_optimized_array_object(ubjson_parser_t* parser, const char* key,
                                              value_t* v) {
  ret_t ret = RET_OK;
  uint32_t type = parser->optimized_type;
  parser->optimized_type = 0;

  if (type == UBJSON_MARKER_UINT8) {
    uint8_t* p = v->value.binary_data.data;
    for (uint32_t i = 0; i < v->value.binary_data.size; i++) {
      ret = tk_object_set_prop_uint8(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_INT8) {
    int8_t* p = v->value.binary_data.data;
    for (uint32_t i = 0; i < v->value.binary_data.size; i++) {
      ret = tk_object_set_prop_int8(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_INT16) {
    int16_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int16_t);
    for (uint32_t i = 0; i < count; i++) {
      ret = tk_object_set_prop_int16(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_INT32) {
    int32_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int32_t);
    for (uint32_t i = 0; i < count; i++) {
      ret = tk_object_set_prop_int32(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_INT64) {
    int64_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int64_t);
    for (uint32_t i = 0; i < count; i++) {
      ret = tk_object_set_prop_int64(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_FLOAT32) {
    float* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(float);
    for (uint32_t i = 0; i < count; i++) {
      ret = tk_object_set_prop_float(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else if (type == UBJSON_MARKER_FLOAT64) {
    double* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(double);
    for (uint32_t i = 0; i < count; i++) {
      ret = tk_object_set_prop_double(parser->obj, key, p[i]);
      return_value_if_fail(ret == RET_OK, ret);
    }
  } else {
    return_value_if_fail(!"not support!", RET_FAIL);
  }
  return ret;
}

static ret_t ubjson_on_key_value_object(void* ctx, const char* key, value_t* v) {
  ret_t ret = RET_OK;
  ubjson_parser_t* parser = (ubjson_parser_t*)ctx;

  if (key == NULL) {
    /*for array append*/
    key = "-1";
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_BEGIN || token == UBJSON_MARKER_ARRAY_BEGIN) {
      tk_object_t* obj =
          token == UBJSON_MARKER_OBJECT_BEGIN ? object_default_create() : object_array_create();
      return_value_if_fail(obj != NULL, RET_OOM);

      if (parser->root == NULL) {
        tk_object_ref(obj);
        parser->root = obj;
      }

      if (parser->obj != NULL) {
        ret = tk_object_set_prop_object(parser->obj, key, obj);
      }

      ret = ubjson_parser_push(parser, obj);
      tk_object_unref(obj);
    } else if (token == UBJSON_MARKER_OBJECT_END || token == UBJSON_MARKER_ARRAY_END) {
      ret = ubjson_parser_pop(parser);

    } else if (token == UBJSON_MARKER_UINT8 || token == UBJSON_MARKER_INT8 ||
               token == UBJSON_MARKER_INT16 || token == UBJSON_MARKER_INT32 ||
               token == UBJSON_MARKER_INT64 || token == UBJSON_MARKER_FLOAT32 ||
               token == UBJSON_MARKER_FLOAT64) {
      parser->optimized_type = token;
    } else {
      assert(!"not supported");
      ret = RET_NOT_IMPL;
    }
  } else {
    if (parser->optimized_type != 0) {
      return_value_if_fail(v->type == VALUE_TYPE_BINARY, RET_FAIL);

      ubjson_on_optimized_array_object(parser, key, v);
      ret = ubjson_parser_pop(parser);

    } else {
      ret = tk_object_set_prop(parser->obj, key, v);
    }
  }

  return ret;
}

static ret_t ubjson_do_parse_key_value(ubjson_parser_t* parser, int32_t len) {
  value_t value;
  value_t* v = &value;
  str_t* str = &(parser->temp);
  ubjson_reader_t* reader = &(parser->reader);

  str_extend(str, len + 1);
  return_value_if_fail(ubjson_reader_read_data(reader, str->str, len) == RET_OK, RET_FAIL);
  str->str[len] = '\0';

  if (ubjson_parser_read(parser, &value) != RET_OK) {
    return RET_DONE;
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);
    if (token == UBJSON_MARKER_ARRAY_BEGIN) {
      ubjson_parser_on_key_value(parser, str->str, v);
      return ubjson_do_parse_array(parser);
    } else if (token == UBJSON_MARKER_OBJECT_BEGIN) {
      ubjson_parser_on_key_value(parser, str->str, v);
      return ubjson_do_parse_object(parser);
    } else {
      assert(!"invalid format");
      return RET_BAD_PARAMS;
    }
  } else {
    ubjson_parser_on_key_value(parser, str->str, v);
  }

  return RET_OK;
}

static ret_t ubjson_do_parse_object(ubjson_parser_t* parser) {
  value_t value;

  while (TRUE) {
    value_t* v = &value;
    if (ubjson_parser_read(parser, &value) != RET_OK) {
      break;
    }

    if (v->type == VALUE_TYPE_TOKEN) {
      if (value_token(v) == UBJSON_MARKER_OBJECT_END) {
        ubjson_parser_on_key_value(parser, NULL, v);
        return RET_OK;
      }

      assert(!"invalid format");
      return RET_BAD_PARAMS;
    }

    return_value_if_fail(v->type == VALUE_TYPE_UINT8 || v->type == VALUE_TYPE_INT8 ||
                             v->type == VALUE_TYPE_INT16 || v->type == VALUE_TYPE_INT32,
                         RET_BAD_PARAMS);

    return_value_if_fail(ubjson_do_parse_key_value(parser, value_int(v)) == RET_OK, RET_FAIL);
  }

  return RET_OK;
}

static ret_t ubjson_do_parse_array(ubjson_parser_t* parser) {
  value_t value;

  while (TRUE) {
    value_t* v = &value;
    if (ubjson_parser_read(parser, &value) != RET_OK) {
      break;
    }

    if (v->type == VALUE_TYPE_TOKEN) {
      uint32_t token = value_token(v);
      if (token == UBJSON_MARKER_ARRAY_END) {
        ubjson_parser_on_key_value(parser, NULL, v);
        return RET_OK;
      } else if (token == UBJSON_MARKER_OBJECT_BEGIN) {
        ubjson_parser_on_key_value(parser, NULL, v);
        return_value_if_fail(ubjson_do_parse_object(parser) == RET_OK, RET_FAIL);
        continue;
      } else if (token == UBJSON_MARKER_ARRAY_BEGIN) {
        ubjson_parser_on_key_value(parser, NULL, v);
        ubjson_do_parse_array(parser);
        continue;
      } else {
        ubjson_parser_on_key_value(parser, NULL, v); /* 上报优化数组的类型 */

        return_value_if_fail(ubjson_parser_read(parser, &value) == RET_OK, RET_FAIL);
        ubjson_parser_on_key_value(parser, NULL, v);
        return RET_OK;
      }
    }

    ubjson_parser_on_key_value(parser, NULL, v);
  }

  return RET_OK;
}

ret_t ubjson_do_parse(ubjson_parser_t* parser) {
  value_t value;
  value_t* v = &value;

  if (ubjson_parser_read(parser, &value) != RET_OK) {
    return RET_DONE;
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    if (value_token(v) == UBJSON_MARKER_OBJECT_BEGIN) {
      ubjson_parser_on_key_value(parser, NULL, v);
      return ubjson_do_parse_object(parser);
    } else if (value_token(v) == UBJSON_MARKER_ARRAY_BEGIN) {
      ubjson_parser_on_key_value(parser, NULL, v);
      return ubjson_do_parse_array(parser);
    }
  }

  assert(!"invalid format");
  return RET_BAD_PARAMS;
}

tk_object_t* ubjson_to_object(void* data, uint32_t size) {
  ubjson_parser_t parser;
  return_value_if_fail(data != NULL && size > 0, NULL);

  ubjson_parser_init(&parser, data, size, ubjson_on_key_value_object, &parser);
  ubjson_do_parse(&parser);
  ubjson_parser_deinit(&parser);

  return parser.root;
}

typedef struct {
  int32_t level;
  uint32_t optimized_type;
} dump_ctx_t;

static ret_t ubjson_on_optimized_array_dump(dump_ctx_t* ctx, const char* key, value_t* v) {
  uint32_t type = ctx->optimized_type;
  ctx->optimized_type = 0;

  if (type == UBJSON_MARKER_UINT8 || type == UBJSON_MARKER_INT8) {
    uint8_t* p = v->value.binary_data.data;
    for (uint32_t i = 0; i < v->value.binary_data.size - 1; i++) {
      log_debug("0x%02x, ", p[i]);
    }
    log_debug("0x%02x", p[v->value.binary_data.size - 1]);
  } else if (type == UBJSON_MARKER_INT16) {
    int16_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int16_t);
    for (uint32_t i = 0; i < count - 1; i++) {
      log_debug("0x%04x, ", p[i]);
    }
    log_debug("0x%04x", p[count - 1]);
  } else if (type == UBJSON_MARKER_INT32) {
    int32_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int32_t);
    for (uint32_t i = 0; i < count - 1; i++) {
      log_debug("0x%08x, ", p[i]);
    }
    log_debug("0x%08x", p[count - 1]);
  } else if (type == UBJSON_MARKER_INT64) {
    int64_t* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(int64_t);
    for (uint32_t i = 0; i < count - 1; i++) {
      log_debug("0x%016"PRIx64", ", p[i]);
    }
    log_debug("0x%016"PRIx64, p[count - 1]);
  } else if (type == UBJSON_MARKER_FLOAT32) {
    float* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(float);
    for (uint32_t i = 0; i < count - 1; i++) {
      log_debug("%f, ", p[i]);
    }
    log_debug("%f", p[count - 1]);
  } else if (type == UBJSON_MARKER_FLOAT64) {
    double* p = v->value.binary_data.data;
    uint32_t count = v->value.binary_data.size / sizeof(double);
    for (uint32_t i = 0; i < count - 1; i++) {
      log_debug("%f, ", p[i]);
    }
    log_debug("%f", p[count - 1]);
  } else {
    return_value_if_fail(!"invalid format", RET_NOT_IMPL);
  }

  return RET_OK;
}

static ret_t ubjson_on_key_value_dump(void* ctx, const char* key, value_t* v) {
  int32_t i = 0;
  ret_t ret = RET_OK;
  dump_ctx_t* dump_ctx = (dump_ctx_t*)ctx;

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_END) {
      dump_ctx->level = dump_ctx->level - 1;
    }
  }

  for (i = 0; i < dump_ctx->level; i++) {
    log_debug(" ");
  }

  if (key != NULL) {
    log_debug("\"%s\":", key);
  }

  if (v->type == VALUE_TYPE_TOKEN) {
    uint32_t token = value_token(v);

    if (token == UBJSON_MARKER_OBJECT_BEGIN) {
      log_debug("{\n");
      dump_ctx->level = dump_ctx->level + 1;
    } else if (token == UBJSON_MARKER_OBJECT_END) {
      log_debug("},\n");
    } else if (token == UBJSON_MARKER_ARRAY_BEGIN) {
      log_debug("[");
    } else if (token == UBJSON_MARKER_ARRAY_END) {
      log_debug("],\n");
    } else if (token == UBJSON_MARKER_UINT8 || token == UBJSON_MARKER_INT8 ||
               token == UBJSON_MARKER_INT16 || token == UBJSON_MARKER_INT32 ||
               token == UBJSON_MARKER_INT64 || token == UBJSON_MARKER_FLOAT32 ||
               token == UBJSON_MARKER_FLOAT64) {
      dump_ctx->optimized_type = token;
    } else {
      assert(!"not supported");
      ret = RET_NOT_IMPL;
    }
  } else {
    if (v->type == VALUE_TYPE_BOOL) {
      log_debug("%s", value_bool(v) ? "true" : "false");
    } else if (v->type == VALUE_TYPE_FLOAT) {
      log_debug("%f", value_float32(v));
    } else if (v->type == VALUE_TYPE_DOUBLE) {
      log_debug("%lf", value_double(v));
    } else if (v->type == VALUE_TYPE_STRING) {
      log_debug("\"%s\"", value_str(v));
    } else if (v->type == VALUE_TYPE_POINTER) {
      log_debug("\"%p\"", value_pointer(v));
    } else if (v->type == VALUE_TYPE_BINARY) {
      if (dump_ctx->optimized_type != 0) {
        ubjson_on_optimized_array_dump(dump_ctx, key, v);
        log_debug("],\n");
        return ret;
      } else {
        uint32_t i = 0;
        binary_data_t* data = value_binary_data(v);

        log_debug("{");
        for (i = 0; i < data->size - 1; i++) {
          log_debug("0x%02x, ", ((uint8_t*)data->data)[i]);
        }
        log_debug("0x%02x}", ((uint8_t*)data->data)[i]);
      }

    } else {
      log_debug("%d", value_int(v));
    }

    if (key != NULL) {
      log_debug(",\n");
    } else {
      log_debug(", ");
    }
  }

  return ret;
}

ret_t ubjson_parse(void* data, uint32_t size, ubjson_on_key_value_t on_key_value, void* ctx) {
  ubjson_parser_t parser;
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  ubjson_parser_init(&parser, data, size, on_key_value, ctx);
  ubjson_do_parse(&parser);
  ubjson_parser_deinit(&parser);

  return RET_OK;
}

ret_t ubjson_dump(void* data, uint32_t size) {
  dump_ctx_t ctx = {0};

  return ubjson_parse(data, size, ubjson_on_key_value_dump, &ctx);
}
