#include "tkc/mem.h"
#include "tkc/mem_allocator_pool.h"
#include "tkc/mem_allocator_simple.h"

#include "tkc/mem_allocator_composite.h"

static void allocator_test_simple(void) {
  char buff[64];
  void* addr1 = NULL;
  void* addr2 = NULL;
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));

  assert(simple.info.buffer == buff);
  assert(simple.info.size == sizeof(buff));
  addr1 = MEM_ALLOCATOR_ALLOC(allocator, 10);
  assert(addr1 != NULL);
  assert(simple.info.used_block_nr == 1);

  addr2 = MEM_ALLOCATOR_ALLOC(allocator, 10);
  assert(addr2 != NULL);
  assert(simple.info.used_block_nr == 2);

  addr1 = MEM_ALLOCATOR_REALLOC(allocator, addr1, 12);
  assert(simple.info.used_block_nr == 2);

  addr2 = MEM_ALLOCATOR_REALLOC(allocator, addr2, 12);
  assert(simple.info.used_block_nr == 2);

  MEM_ALLOCATOR_FREE(allocator, addr1);
  assert(simple.info.used_block_nr == 1);

  MEM_ALLOCATOR_FREE(allocator, addr2);
  assert(simple.info.used_block_nr == 0);
  assert(simple.info.used_bytes == 0);
  assert(simple.info.used_max_block_nr == 2);
  assert(simple.info.free_list->size == sizeof(buff));

  mem_allocator_destroy(allocator);
}

void allocator_test_basic_ex(mem_allocator_t* allocator) {
  uint32_t i = 0;
  void* addr = NULL;
  void* addrs[100];

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    addr = MEM_ALLOCATOR_ALLOC(allocator, i + 1);
    assert(addr != NULL);
    addr = MEM_ALLOCATOR_REALLOC(allocator, addr, i + 1);
    assert(addr != NULL);
    addr = MEM_ALLOCATOR_REALLOC(allocator, addr, i + 5);
    assert(addr != NULL);
    addrs[i] = addr;
  }

  for (i = 0; i < ARRAY_SIZE(addrs); i++) {
    MEM_ALLOCATOR_FREE(allocator, addrs[i]);
  }
}

void allocator_test_basic() {
  uint32_t i = 0;
  char buff[102400];
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  mem_allocator_pool_t pool;
  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  allocator_test_basic_ex(allocator);

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
  mem_allocator_destroy(allocator);

  return;
}

void allocator_test_rand_ex(mem_allocator_t* allocator) {
  uint32_t k = 0;
  uint32_t i = 0;
  void* addr = NULL;
  void* addrs[1000];

  for (k = 0; k < 1000; k++) {
    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      uint32_t size = random() % 100;
      addr = MEM_ALLOCATOR_ALLOC(allocator, size);
      assert(addr != NULL);
      addr = MEM_ALLOCATOR_REALLOC(allocator, addr, size);
      assert(addr != NULL);
      addr = MEM_ALLOCATOR_REALLOC(allocator, addr, size + 10);
      assert(addr != NULL);
      addrs[i] = addr;
    }

    for (i = 0; i < ARRAY_SIZE(addrs); i++) {
      MEM_ALLOCATOR_FREE(allocator, addrs[i]);
    }
  }
}

void allocator_test_rand() {
  uint32_t i = 0;
  char buff[1000 * 1000];
  mem_allocator_simple_t simple;
  mem_allocator_t* allocator = mem_allocator_simple_init(&simple, buff, sizeof(buff));
  mem_allocator_pool_t pool;

  allocator = mem_allocator_pool_init(&pool, allocator, 10, 10, 10, 10, 10);

  allocator_test_rand_ex(allocator);

  for (i = 0; i < TK_MEM_POOLS_NR; i++) {
    assert(pool.pools[i]->used == 0);
  }
  mem_allocator_destroy(allocator);
}

void allocator_test_composite0() {
  char mem1[100 * 1000];
  char mem2[1000 * 1000];
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator =
      mem_allocator_composite_init(&composite, mem1, sizeof(mem1), mem2, sizeof(mem2), NULL);

  allocator_test_basic_ex(allocator);
  allocator_test_rand_ex(allocator);
  mem_allocator_destroy(allocator);
}

void allocator_test_composite1() {
  char mem1[36];
  char mem2[64];
  char mem3[1280];
  char* addr = NULL;
  mem_allocator_composite_t composite;
  mem_allocator_t* allocator = mem_allocator_composite_init(&composite, mem1, sizeof(mem1), mem2,
                                                            sizeof(mem2), mem3, sizeof(mem3), NULL);

  /*mem1*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 16);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 16 + 2);
  assert(addr >= mem1);
  assert(addr < (mem1 + sizeof(mem1)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  /*mem2*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 36);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 36);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 30);
  assert(addr >= mem2);
  assert(addr < (mem2 + sizeof(mem2)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  /*mem3*/
  addr = MEM_ALLOCATOR_ALLOC(allocator, 66);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));

  MEM_ALLOCATOR_FREE(allocator, addr);
  addr = MEM_ALLOCATOR_ALLOC(allocator, 66);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  addr = MEM_ALLOCATOR_REALLOC(allocator, addr, 66 + 2);
  assert(addr >= mem3);
  assert(addr < (mem3 + sizeof(mem3)));
  assert(mem_allocator_composite_is_valid_addr(allocator, addr));

  mem_allocator_destroy(allocator);
  return;
}

void allocator_test() {
  allocator_test_composite1();
  allocator_test_basic();
  allocator_test_rand();
  allocator_test_simple();
}
