﻿#include "gtest/gtest.h"
#include "streams/shdlc/shdlc_helper.h"
#include "streams/mem/istream_mem.h"

TEST(SHDLC, header) {
  shdlc_header_t header = {0};
  header.s.type = 1;
  header.s.seqno = 3;
  header.s.reserve = 0;
  ASSERT_EQ(header.data, 0x19);
  ASSERT_EQ(sizeof(header), 1u);
}

TEST(SHDLC, ack) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_ack(wb, 1), RET_OK);
  ASSERT_EQ(wb->cursor, 5u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_ACK);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  ASSERT_EQ(wb->data[2], 0xdd);
  ASSERT_EQ(wb->data[3], 0xa0);
  ASSERT_EQ(wb->data[4], SHDLC_FLAG);

  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_OK);
  ASSERT_EQ(wb_r->cursor, 1u);

  header.data = wb_r->data[0];
  ASSERT_EQ(header.s.type, SHDLC_ACK);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}

TEST(SHDLC, nack) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_nack(wb, 1), RET_OK);
  ASSERT_EQ(wb->cursor, 5u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_NACK);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  ASSERT_EQ(wb->data[2], 0x54);
  ASSERT_EQ(wb->data[3], 0xb1);
  ASSERT_EQ(wb->data[4], SHDLC_FLAG);

  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_OK);
  ASSERT_EQ(wb_r->cursor, 1u);

  header.data = wb_r->data[0];
  ASSERT_EQ(header.s.type, SHDLC_NACK);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}

TEST(SHDLC, data_ok) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_data(wb, 1, FALSE, "abc", 3), RET_OK);
  ASSERT_EQ(wb->cursor, 8u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);
  ASSERT_EQ(header.s.compressed, FALSE);

  ASSERT_EQ(wb->data[2], 'a');
  ASSERT_EQ(wb->data[3], 'b');
  ASSERT_EQ(wb->data[4], 'c');
  ASSERT_EQ(wb->data[5], 0xab);
  ASSERT_EQ(wb->data[6], 0xa2);
  ASSERT_EQ(wb->data[7], SHDLC_FLAG);

  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_OK);
  ASSERT_EQ(wb_r->cursor, 4u);

  header.data = wb_r->data[0];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  ASSERT_EQ(wb_r->data[1], 'a');
  ASSERT_EQ(wb_r->data[2], 'b');
  ASSERT_EQ(wb_r->data[3], 'c');

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}

TEST(SHDLC, data_crc_error) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_data(wb, 3, TRUE, "abc", 3), RET_OK);
  ASSERT_EQ(wb->cursor, 8u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 3);
  ASSERT_EQ(header.s.reserve, 0);
  ASSERT_EQ(header.s.compressed, TRUE);

  ASSERT_EQ(wb->data[2], 'a');
  ASSERT_EQ(wb->data[3], 'b');
  ASSERT_EQ(wb->data[4], 'c');
  ASSERT_EQ(wb->data[5], 0xbd);
  ASSERT_EQ(wb->data[6], 0x77);
  ASSERT_EQ(wb->data[7], SHDLC_FLAG);

  wb->data[4] = 'd';
  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_CRC);
  ASSERT_EQ(wb_r->cursor, 4u);

  header.data = wb_r->data[0];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 3);
  ASSERT_EQ(header.s.reserve, 0);

  ASSERT_EQ(wb_r->data[1], 'a');
  ASSERT_EQ(wb_r->data[2], 'b');
  ASSERT_EQ(wb_r->data[3], 'd');

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}

TEST(SHDLC, data_error) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_data(wb, 5, FALSE, "abc", 3), RET_OK);
  ASSERT_EQ(wb->cursor, 8u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 5);
  ASSERT_EQ(header.s.reserve, 0);
  ASSERT_EQ(header.s.compressed, FALSE);

  ASSERT_EQ(wb->data[2], 'a');
  ASSERT_EQ(wb->data[3], 'b');
  ASSERT_EQ(wb->data[4], 'c');
  ASSERT_EQ(wb->data[5], 0xf8);
  ASSERT_EQ(wb->data[6], 0x2d);
  ASSERT_EQ(wb->data[7], SHDLC_FLAG);

  wb->data[7] = 'd';
  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_TIMEOUT);

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}

TEST(SHDLC, data_escape) {
  wbuffer_t wbuff;
  wbuffer_t wbuff_r;
  shdlc_header_t header = {0};
  tk_istream_t* istream = NULL;
  wbuffer_t* wb = wbuffer_init_extendable(&wbuff);
  wbuffer_t* wb_r = wbuffer_init_extendable(&wbuff_r);

  ASSERT_EQ(shdlc_write_data(wb, 1, FALSE, "x\x7dy", 3), RET_OK);
  ASSERT_EQ(wb->cursor, 9u);
  ASSERT_EQ(wb->data[0], SHDLC_FLAG);

  header.data = wb->data[1];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);
  ASSERT_EQ(header.s.compressed, FALSE);

  ASSERT_EQ(wb->data[2], 'x');
  ASSERT_EQ(wb->data[3], '\x7d');
  ASSERT_EQ(wb->data[4], 0x5d);
  ASSERT_EQ(wb->data[5], 'y');
  ASSERT_EQ(wb->data[6], 0xa2);
  ASSERT_EQ(wb->data[7], 0x12);
  ASSERT_EQ(wb->data[8], SHDLC_FLAG);

  istream = tk_istream_mem_create(wb->data, wb->cursor, 0, FALSE);

  ASSERT_EQ(shdlc_read_data(istream, wb_r, 1000), RET_OK);
  ASSERT_EQ(wb_r->cursor, 4u);

  header.data = wb_r->data[0];
  ASSERT_EQ(header.s.type, SHDLC_DATA);
  ASSERT_EQ(header.s.seqno, 1);
  ASSERT_EQ(header.s.reserve, 0);

  ASSERT_EQ(wb_r->data[1], 'x');
  ASSERT_EQ(wb_r->data[2], '\x7d');
  ASSERT_EQ(wb_r->data[3], 'y');

  TK_OBJECT_UNREF(istream);
  wbuffer_deinit(wb);
  wbuffer_deinit(wb_r);
}
