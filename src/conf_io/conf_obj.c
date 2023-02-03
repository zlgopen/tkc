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
 * 2020-05-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/object.h"
#include "tkc/named_value.h"
#include "conf_io/conf_obj.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

typedef struct _conf_obj_t {
  tk_object_t object;

  /*private*/
  char* url;
  char* default_url;
  conf_doc_t* doc;
  conf_doc_save_t save;
  conf_doc_load_t load;
  bool_t readonly;
  bool_t modified;
} conf_obj_t;

static conf_obj_t* conf_obj_cast(tk_object_t* obj);
#define CONF_OBJ(obj) conf_obj_cast((tk_object_t*)obj)

static ret_t conf_obj_move_up(tk_object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->modified = TRUE;
  return conf_doc_move_up(o->doc, name);
}

static ret_t conf_obj_move_down(tk_object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->modified = TRUE;
  return conf_doc_move_down(o->doc, name);
}

static ret_t conf_obj_remove_prop(tk_object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  o->modified = TRUE;
  return conf_doc_remove(o->doc, name);
}

static ret_t conf_obj_clear(tk_object_t* obj, const char* name) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  o->modified = TRUE;
  return conf_doc_clear(o->doc, name);
}

static ret_t conf_obj_foreach_node(conf_node_t* root, tk_visit_t on_prop, void* ctx) {
  named_value_t nv;
  ret_t ret = RET_FAIL;
  conf_node_t* iter = conf_node_get_first_child(root);
  return_value_if_fail(iter != NULL, RET_BAD_PARAMS);

  for (; iter != NULL; iter = iter->next) {
    nv.name = (char*)conf_node_get_name(iter);
    if (conf_node_get_value(iter, &(nv.value)) == RET_OK) {
      ret = on_prop(ctx, &nv);
      if (ret != RET_OK) {
        break;
      }
    } else {
      log_debug("skip object\n");
    }
  }

  return ret;
}

static ret_t conf_obj_foreach(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL && o->doc != NULL && o->doc->root != NULL, RET_BAD_PARAMS);

  return conf_obj_foreach_node(o->doc->root, on_prop, ctx);
}

static ret_t conf_obj_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  if (tk_str_eq(name, CONF_OBJ_PROP_DEFAULT_URL)) {
    o->default_url = tk_str_copy(o->default_url, value_str(v));
    return RET_OK;
  }

  o->modified = TRUE;
  return conf_doc_set(o->doc, name, v);
}

static ret_t conf_obj_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CONF_OBJ_PROP_URL)) {
    value_set_str(v, o->url);
    return RET_OK;
  } else if (tk_str_eq(name, CONF_OBJ_PROP_DEFAULT_URL)) {
    value_set_str(v, o->default_url);
    return RET_OK;
  }

  return conf_doc_get(o->doc, name, v);
}

ret_t conf_obj_save(tk_object_t* obj) {
  ret_t ret = RET_FAIL;
  data_writer_t* writer = NULL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  writer = data_writer_factory_create_writer(data_writer_factory(), o->url);
  return_value_if_fail(writer != NULL, RET_FAIL);

  ret = o->save(o->doc, writer);
  data_writer_flush(writer);
  data_writer_destroy(writer);

  if (ret == RET_OK) {
    o->modified = FALSE;
  }

  return ret;
}

static ret_t conf_obj_load(tk_object_t* obj) {
  data_reader_t* reader = NULL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  reader = data_reader_factory_create_reader(data_reader_factory(), o->url);
  if (reader == NULL) {
    return RET_FAIL;
  }

  o->doc = o->load(reader);
  data_reader_destroy(reader);

  if (o->doc == NULL) {
    o->doc = conf_doc_create(10);
  }

  return RET_OK;
}

static ret_t conf_obj_load_or_create(tk_object_t* obj, bool_t create_if_not_exist) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_FAIL);

  if (o->url != NULL) {
    conf_obj_load(obj);
  }

  if (o->doc == NULL && create_if_not_exist) {
    o->doc = conf_doc_create(20);
  }

  if (o->doc != NULL) {
    if (o->doc->root == NULL) {
      o->doc->root = conf_doc_create_node(o->doc, CONF_NODE_ROOT_NAME);
    }
  }

  return RET_OK;
}

static ret_t conf_obj_reload(tk_object_t* obj) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  conf_doc_destroy(o->doc);
  o->doc = NULL;

  return conf_obj_load_or_create(obj, FALSE);
}

static bool_t conf_obj_can_exec(tk_object_t* obj, const char* name, const char* args) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return FALSE;
  }

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_RELOAD)) {
    return TRUE;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_MOVE_UP)) {
    return conf_doc_exists(o->doc, args) && !conf_doc_is_first(o->doc, args);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_MOVE_DOWN)) {
    return conf_doc_exists(o->doc, args) && !conf_doc_is_last(o->doc, args);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    return conf_doc_exists(o->doc, args);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    return conf_doc_exists(o->doc, args);
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    return conf_doc_exists(o->doc, args);
  }

  return FALSE;
}

static ret_t conf_obj_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_IMPL;
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->readonly) {
    return RET_NOT_IMPL;
  }

  if (tk_str_ieq(name, TK_OBJECT_CMD_SAVE)) {
    if (tk_str_eq(args, "force")) {
      ret = conf_obj_save(obj);
    } else if (o->modified) {
      ret = conf_obj_save(obj);
    } else {
      ret = RET_NOT_MODIFIED;
    }
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_RELOAD)) {
    conf_obj_reload(obj);
    ret = RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_MOVE_UP)) {
    if (conf_obj_move_up(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_MOVE_DOWN)) {
    if (conf_obj_move_down(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_CLEAR)) {
    if (conf_obj_clear(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_REMOVE)) {
    if (conf_obj_remove_prop(obj, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else if (tk_str_ieq(name, TK_OBJECT_CMD_ADD)) {
    if (conf_doc_add_child(o->doc, args) == RET_OK) {
      ret = RET_ITEMS_CHANGED;
    } else {
      ret = RET_FAIL;
    }
  } else {
    return RET_NOT_IMPL;
  }

  if (ret == RET_ITEMS_CHANGED) {
    emitter_dispatch_simple_event(EMITTER(obj), EVT_PROPS_CHANGED);
    emitter_dispatch_simple_event(EMITTER(obj), EVT_ITEMS_CHANGED);
  }

  return RET_OK;
}

static ret_t conf_obj_destroy(tk_object_t* obj) {
  conf_obj_t* o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  conf_doc_destroy(o->doc);
  TKMEM_FREE(o->url);
  TKMEM_FREE(o->default_url);
  o->doc = NULL;

  return RET_OK;
}

conf_doc_t* conf_obj_get_doc(tk_object_t* conf) {
  conf_obj_t* o = CONF_OBJ(conf);
  return_value_if_fail(o != NULL, NULL);

  return_value_if_fail(conf != NULL, NULL);

  return o->doc;
}

static const object_vtable_t s_conf_obj_vtable = {.type = "conf_obj",
                                                  .desc = "conf_obj",
                                                  .size = sizeof(conf_obj_t),
                                                  .is_collection = FALSE,
                                                  .exec = conf_obj_exec,
                                                  .can_exec = conf_obj_can_exec,
                                                  .remove_prop = conf_obj_remove_prop,
                                                  .get_prop = conf_obj_get_prop,
                                                  .set_prop = conf_obj_set_prop,
                                                  .foreach_prop = conf_obj_foreach,
                                                  .on_destroy = conf_obj_destroy};

static conf_obj_t* conf_obj_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_conf_obj_vtable, NULL);

  return (conf_obj_t*)obj;
}

tk_object_t* conf_obj_create(conf_doc_save_t save, conf_doc_load_t load, const char* url,
                             bool_t create_if_not_exist) {
  conf_obj_t* o = NULL;
  tk_object_t* obj = NULL;
  return_value_if_fail(save != NULL && load != NULL, NULL);
  return_value_if_fail((url != NULL && *url) || create_if_not_exist, NULL);

  obj = tk_object_create(&s_conf_obj_vtable);
  o = CONF_OBJ(obj);
  return_value_if_fail(o != NULL, NULL);

  o->save = save;
  o->load = load;
  if (url != NULL) {
    o->url = tk_strdup(url);
  }

  if (o->url == NULL && url != NULL) {
    TK_OBJECT_UNREF(o);
  }
  return_value_if_fail(o != NULL, NULL);

  conf_obj_load_or_create(obj, create_if_not_exist);

  if (o->doc == NULL || o->doc->root == NULL) {
    TKMEM_FREE(o->url);
    TK_OBJECT_UNREF(obj);
  }

  return obj;
}

ret_t conf_obj_set_readonly(tk_object_t* conf, bool_t readonly) {
  conf_obj_t* o = CONF_OBJ(conf);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  o->readonly = readonly;

  return RET_OK;
}

typedef struct _conf_sub_obj_t {
  tk_object_t object;
  conf_obj_t* conf;
  conf_node_t* root;
  conf_node_t* real_root;
} conf_sub_obj_t;

static conf_sub_obj_t* conf_sub_obj_cast(tk_object_t* obj);
#define CONF_SUB_OBJ(obj) conf_sub_obj_cast((tk_object_t*)obj)

static ret_t conf_sub_obj_exec(tk_object_t* obj, const char* name, const char* args) {
  /*FIXME:暂时不支持*/
  return RET_NOT_IMPL;
}

static bool_t conf_sub_obj_can_exec(tk_object_t* obj, const char* name, const char* args) {
  /*FIXME:暂时不支持*/
  return FALSE;
}

static ret_t conf_sub_obj_remove_prop(tk_object_t* obj, const char* name) {
  ret_t ret = RET_FAIL;
  conf_sub_obj_t* o = CONF_SUB_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->conf->doc->root = o->root;
  ret = conf_obj_remove_prop(TK_OBJECT(o->conf), name);
  o->conf->doc->root = o->real_root;

  return ret;
}

static ret_t conf_sub_obj_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_FAIL;
  conf_sub_obj_t* o = CONF_SUB_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->conf->doc->root = o->root;
  ret = conf_obj_get_prop(TK_OBJECT(o->conf), name, v);
  o->conf->doc->root = o->real_root;

  return ret;
}

static ret_t conf_sub_obj_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_FAIL;
  conf_sub_obj_t* o = CONF_SUB_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->conf->doc->root = o->root;
  ret = conf_obj_set_prop(TK_OBJECT(o->conf), name, v);
  o->conf->doc->root = o->real_root;

  return ret;
}

static ret_t conf_sub_obj_foreach(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  conf_sub_obj_t* o = CONF_SUB_OBJ(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return conf_obj_foreach_node(o->root, on_prop, ctx);
}

static ret_t conf_sub_obj_destroy(tk_object_t* obj) {
  conf_sub_obj_t* o = CONF_SUB_OBJ(obj);
  TK_OBJECT_UNREF(o->conf);
  o->root = NULL;
  o->real_root = NULL;

  return RET_OK;
}

static const object_vtable_t s_conf_sub_obj_vtable = {.type = "conf_sub_obj",
                                                      .desc = "conf_sub_obj",
                                                      .size = sizeof(conf_sub_obj_t),
                                                      .is_collection = FALSE,
                                                      .exec = conf_sub_obj_exec,
                                                      .can_exec = conf_sub_obj_can_exec,
                                                      .remove_prop = conf_sub_obj_remove_prop,
                                                      .get_prop = conf_sub_obj_get_prop,
                                                      .set_prop = conf_sub_obj_set_prop,
                                                      .foreach_prop = conf_sub_obj_foreach,
                                                      .on_destroy = conf_sub_obj_destroy};

static conf_sub_obj_t* conf_sub_obj_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_conf_sub_obj_vtable, NULL);

  return (conf_sub_obj_t*)obj;
}

tk_object_t* conf_sub_obj_create(tk_object_t* conf, const char* path) {
  tk_object_t* obj = NULL;
  conf_sub_obj_t* o = NULL;
  conf_node_t* root = NULL;
  conf_obj_t* conf_obj = NULL;
  return_value_if_fail(conf != NULL && path != NULL, NULL);

  if (conf->vt == &s_conf_sub_obj_vtable) {
    conf_sub_obj_t* sub = CONF_SUB_OBJ(conf);
    conf_obj = sub->conf;
    return_value_if_fail(conf_obj->doc != NULL && conf_obj->doc->root != NULL, NULL);
    root = conf_doc_find_node(conf_obj->doc, sub->root, path, FALSE);
    return_value_if_fail(root != NULL, NULL);
  } else {
    conf_obj = CONF_OBJ(conf);
    root = conf_doc_find_node(conf_obj->doc, conf_obj->doc->root, path, FALSE);
    return_value_if_fail(root != NULL, NULL);
  }

  obj = tk_object_create(&s_conf_sub_obj_vtable);
  o = CONF_SUB_OBJ(obj);
  return_value_if_fail(o != NULL, NULL);

  o->conf = conf_obj;
  o->root = root;
  o->real_root = conf_obj->doc->root;
  TK_OBJECT_REF(o->conf);

  return obj;
}

tk_object_t* conf_obj_create_sub_object(tk_object_t* conf, const char* path) {
  return conf_sub_obj_create(conf, path);
}
