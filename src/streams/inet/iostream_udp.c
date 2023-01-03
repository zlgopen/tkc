﻿/**
 * File:   iostream_udp.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on socket
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/socket_helper.h"
#include "streams/inet/istream_udp.h"
#include "streams/inet/ostream_udp.h"
#include "streams/inet/iostream_udp.h"

static ret_t tk_iostream_udp_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_iostream_udp_t* iostream_udp = TK_IOSTREAM_UDP(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, iostream_udp->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok1 =
        tk_object_get_prop_bool(TK_OBJECT(iostream_udp->istream), TK_STREAM_PROP_IS_OK, TRUE);
    bool_t is_ok2 =
        tk_object_get_prop_bool(TK_OBJECT(iostream_udp->ostream), TK_STREAM_PROP_IS_OK, TRUE);

    value_set_bool(v, is_ok1 && is_ok2);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_udp_on_destroy(tk_object_t* obj) {
  tk_iostream_udp_t* iostream_udp = TK_IOSTREAM_UDP(obj);

  tk_socket_close(iostream_udp->sock);
  tk_object_unref(TK_OBJECT(iostream_udp->istream));
  tk_object_unref(TK_OBJECT(iostream_udp->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_udp_vtable = {.type = "tk_iostream_udp",
                                                         .desc = "tk_iostream_udp",
                                                         .size = sizeof(tk_iostream_udp_t),
                                                         .get_prop = tk_iostream_udp_get_prop,
                                                         .on_destroy = tk_iostream_udp_on_destroy};

static tk_istream_t* tk_iostream_udp_get_istream(tk_iostream_t* stream) {
  tk_iostream_udp_t* iostream_udp = TK_IOSTREAM_UDP(stream);

  return iostream_udp->istream;
}

static tk_ostream_t* tk_iostream_udp_get_ostream(tk_iostream_t* stream) {
  tk_iostream_udp_t* iostream_udp = TK_IOSTREAM_UDP(stream);

  return iostream_udp->ostream;
}

tk_iostream_t* tk_iostream_udp_create(int sock) {
  tk_object_t* obj = tk_object_create(&s_tk_iostream_udp_vtable);
  tk_iostream_udp_t* iostream_udp = TK_IOSTREAM_UDP(obj);

  if (iostream_udp == NULL) {
    tk_socket_close(sock);
    return_value_if_fail(iostream_udp != NULL, NULL);
  }

  iostream_udp->sock = sock;
  iostream_udp->istream = tk_istream_udp_create(sock);
  iostream_udp->ostream = tk_ostream_udp_create(sock);

  TK_IOSTREAM(obj)->get_istream = tk_iostream_udp_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_udp_get_ostream;

  return TK_IOSTREAM(obj);
}

tk_iostream_t* tk_iostream_udp_create_client_ex(const char* host, int port, const char* local_ip,
                                                int local_port) {
  int sock = 0;
  struct sockaddr_in addr_in;
  tk_iostream_udp_t* iostream_udp = NULL;
  struct sockaddr* addr = socket_resolve(host, port, &addr_in);
  return_value_if_fail(addr != NULL, NULL);

  sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
  return_value_if_fail(sock >= 0, NULL);

  if (local_ip != NULL || local_port > 0) {
    tk_socket_bind_ex(sock, local_ip, local_port);
  }

  iostream_udp = TK_IOSTREAM_UDP(tk_iostream_udp_create(sock));
  return_value_if_fail(iostream_udp != NULL, NULL);

  tk_istream_udp_set_target_with_addr(iostream_udp->istream, addr_in);
  tk_ostream_udp_set_target_with_addr(iostream_udp->ostream, addr_in);

  return TK_IOSTREAM(iostream_udp);
}

tk_iostream_t* tk_iostream_udp_create_client(const char* host, int port) {
  return tk_iostream_udp_create_client_ex(host, port, NULL, -1);
}
