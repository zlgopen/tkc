﻿#include "tkc/matrix.h"
#include "gtest/gtest.h"

TEST(Matrix, basic) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_transform_point(&m, 10, 20, &x, &y);

  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, translate) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_translate(&m, 10, 20);
  matrix_transform_point(&m, 10, 20, &x, &y);

  ASSERT_EQ(x, 20);
  ASSERT_EQ(y, 40);
}

TEST(Matrix, rotate) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_rotate(&m, M_PI);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, -10);
  ASSERT_EQ(y, -20);

  matrix_rotate(&m, M_PI);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, scale) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_scale(&m, 2, 2);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 20);
  ASSERT_EQ(y, 40);

  matrix_scale(&m, 0.5, 0.5);
  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, 10);
  ASSERT_EQ(y, 20);
}

TEST(Matrix, all) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;
  matrix_init(&m);
  matrix_rotate(&m, M_PI / 2);
  matrix_scale(&m, 2, 2);
  matrix_translate(&m, 2, 2);
  matrix_scale(&m, 2, 2);

  matrix_transform_point(&m, 10, 20, &x, &y);
  ASSERT_EQ(x, -76);
  ASSERT_EQ(y, 44);
}

TEST(Matrix, basic1) {
  xy_t x = 0;
  xy_t y = 0;
  xy_t ox = 0;
  xy_t oy = 0;
  matrix_t m;

  matrix_init(&m);
  matrix_translate(&m, -50, -50);
  matrix_rotate(&m, M_PI / 2);
  matrix_translate(&m, 50, 50);
  matrix_translate(&m, ox, oy);

  matrix_transform_point(&m, ox + 50, oy, &x, &y);

  ASSERT_EQ(x, 100);
  ASSERT_EQ(y, 50);
}

TEST(Matrix, basic2) {
  xy_t x = 0;
  xy_t y = 0;
  xy_t ox = 0;
  xy_t oy = 10;
  matrix_t m;

  matrix_init(&m);
  matrix_translate(&m, -50, -50);
  matrix_rotate(&m, M_PI / 2);
  matrix_translate(&m, 50, 50);
  matrix_translate(&m, ox, oy);

  matrix_transform_point(&m, 50, 0, &x, &y);

  ASSERT_EQ(x, 100);
  ASSERT_EQ(y, 60);
}

TEST(Matrix, order) {
  xy_t x = 0;
  xy_t y = 0;
  matrix_t m;

  matrix_init(&m);
  matrix_rotate(&m, M_PI / 2);
  matrix_translate(&m, 100, 0);

  matrix_transform_point(&m, 100, 100, &x, &y);

  ASSERT_EQ(x, 0);
  ASSERT_EQ(y, 100);

  /************************************************/

  matrix_init(&m);
  matrix_translate(&m, 100, 0);
  matrix_rotate(&m, M_PI / 2);

  matrix_transform_point(&m, 100, 100, &x, &y);

  ASSERT_EQ(x, -100);
  ASSERT_EQ(y, 200);
}
