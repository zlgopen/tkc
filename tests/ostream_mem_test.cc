#include <errno.h>
#include <string.h>
#include "streams/mem/ostream_mem.h"
#include "tkc/object.h"
#include "gtest/gtest.h"

TEST(OStreamMem, basic) {
  uint8_t buff[12];

  tk_ostream_t* os = tk_ostream_mem_create(buff, sizeof(buff), 0, FALSE);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"1111", 4), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"1111", 4), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"2222", 4), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"11112222", 8), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"3333", 4), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"111122223333", 12), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"4444", 4), 0);

  tk_object_unref(TK_OBJECT(os));
}

TEST(OStreamMem, packet_size) {
  uint8_t buff[12];
  tk_ostream_t* os = tk_ostream_mem_create(buff, sizeof(buff), 4, FALSE);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"11111", 5), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"1111", 4), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"22222", 5), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"11112222", 8), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"33333", 5), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"111122223333", 12), 0);

  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"4444", 4), 0);

  ASSERT_EQ(tk_ostream_seek(os, 0), RET_OK);
  ASSERT_EQ(tk_ostream_write(os, (uint8_t*)"4444", 4), 4);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"4444", 4), 0);

  tk_object_unref(TK_OBJECT(os));
}

TEST(OStreamMem, write_len) {
  uint8_t buff[12];
  tk_ostream_t* os = tk_ostream_mem_create(buff, sizeof(buff), 4, FALSE);

  ASSERT_EQ(tk_ostream_write_len(os, (uint8_t*)"1111122223333", 12, 3000), 12);
  ASSERT_EQ(memcmp(buff, (uint8_t*)"1111122223333", 12), 0);

  tk_object_unref(TK_OBJECT(os));
}

typedef struct _eagain_ostream_t {
  tk_ostream_t ostream;
  int32_t eagain_left;
  int32_t flush_count;
  uint8_t* buff;
  uint32_t size;
  uint32_t cursor;
} eagain_ostream_t;

#define EAGAIN_OSTREAM(obj) ((eagain_ostream_t*)(obj))

static int32_t eagain_ostream_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  int32_t size = (int32_t)max_size;
  eagain_ostream_t* os = EAGAIN_OSTREAM(stream);

  if (os->eagain_left > 0) {
    os->eagain_left--;
    errno = EAGAIN;
    return -1;
  }

  if (size > (int32_t)(os->size - os->cursor)) {
    size = (int32_t)(os->size - os->cursor);
  }
  if (size > 0) {
    memcpy(os->buff + os->cursor, buff, size);
    os->cursor += size;
  }

  return size;
}

static ret_t eagain_ostream_flush(tk_ostream_t* stream) {
  EAGAIN_OSTREAM(stream)->flush_count++;
  return RET_OK;
}

static const struct eagain_ostream_vtable_t : object_vtable_t {
  eagain_ostream_vtable_t() {
    type = "eagain_ostream";
    desc = "eagain_ostream";
    size = sizeof(eagain_ostream_t);
  }
} s_eagain_ostream_vtable;

TEST(OStreamMem, write_len_eagain_flush) {
  uint8_t buff[12];
  const char* data = "hello123";
  tk_object_t* obj = tk_object_create(&s_eagain_ostream_vtable);
  eagain_ostream_t* mock = EAGAIN_OSTREAM(obj);
  tk_ostream_t* os = TK_OSTREAM(obj);

  memset(buff, 0x00, sizeof(buff));
  mock->eagain_left = 2;
  mock->flush_count = 0;
  mock->buff = buff;
  mock->size = sizeof(buff);
  mock->cursor = 0;
  os->write = eagain_ostream_write;
  os->flush = eagain_ostream_flush;

  ASSERT_EQ(tk_ostream_write_len(os, data, 8, 1000), 8);
  ASSERT_EQ(mock->flush_count, 2);
  ASSERT_EQ(mock->eagain_left, 0);
  ASSERT_EQ(memcmp(buff, data, 8), 0);

  tk_object_unref(obj);
}
