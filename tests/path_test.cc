﻿#include "tkc/path.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(Path, basename) {
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_basename("/a/test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test.bin"));

  ASSERT_EQ(path_basename("test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test.bin"));

  ASSERT_EQ(path_basename("test", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test"));

  ASSERT_EQ(path_basename("", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string(""));
}

TEST(Path, basename_ex) {
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_basename_ex("/a/test.bin", TRUE, result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test"));

  ASSERT_EQ(path_basename_ex("test.bin", TRUE, result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test"));

  ASSERT_EQ(path_basename_ex("test", TRUE, result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("test"));

  ASSERT_EQ(path_basename_ex("", TRUE, result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string(""));
}

TEST(Path, extname_is) {
  ASSERT_EQ(path_extname_is("test.bin", ".bin"), TRUE);
  ASSERT_EQ(path_extname_is("test.Bin", ".bin"), TRUE);
  ASSERT_EQ(path_extname_is("test.jpg", ".JPG"), TRUE);
  ASSERT_EQ(path_extname_is("test.jpg", ".JPG"), TRUE);
  ASSERT_EQ(path_extname_is("test.jpg", NULL), FALSE);
}

TEST(Path, extname) {
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_extname("/a/test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string(".bin"));

  ASSERT_EQ(path_extname("test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string(".bin"));

  ASSERT_EQ(path_extname(".bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string(".bin"));

  ASSERT_EQ(path_extname("bin", result, sizeof(result)), RET_FAIL);
}

TEST(Path, dirname) {
  char result[MAX_PATH + 1] = {0};
  ASSERT_EQ(path_dirname("/a/test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("/a/"));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("/a/b/test.bin", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("/a/b/"));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("test.bin", result, sizeof(result)), RET_FAIL);
  ASSERT_EQ(string(result), string(""));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("e:\\a\\b/c/d", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("e:\\a\\b/c/"));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("e:\\a\\b/c\\d", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("e:\\a\\b/c\\"));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("e:/a/b\\c\\d", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("e:/a/b\\c\\"));

  memset(result, 0x0, sizeof(result));
  ASSERT_EQ(path_dirname("e:/a/b\\c/d", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), string("e:/a/b\\c/"));
}

TEST(Path, exe) {
  char result[MAX_PATH + 1];

  ASSERT_EQ(path_exe(result), RET_OK);
  ASSERT_EQ(strstr(result, "runTest") != NULL, true);
}

string normalPath(const char* str) {
  string s = str;
  int32_t i = 0;
  int32_t n = s.size();

  for (i = 0; i < n; i++) {
    if (s[i] == '/') {
      s[i] = TK_PATH_SEP;
    }
  }

  return s;
}

TEST(Path, normalize) {
  char result[MAX_PATH + 1];

  ASSERT_EQ(path_normalize("/a/b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/b"));

  ASSERT_EQ(path_normalize("/a//b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/b"));

  ASSERT_EQ(path_normalize("/a//./////././/b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/b"));

  ASSERT_EQ(path_normalize("/a/b.exe", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/b.exe"));

  ASSERT_EQ(path_normalize("/a/./b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/b"));

  ASSERT_EQ(path_normalize("./a/./b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("a/b"));

  ASSERT_EQ(path_normalize(".\\a\\.\\b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("a/b"));

  ASSERT_EQ(path_normalize("./a/../b", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("b"));

  ASSERT_EQ(path_normalize("./a/b/../c", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("a/c"));

  ASSERT_EQ(path_normalize("./a/b/../../c", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("c"));

  ASSERT_EQ(path_normalize("/a/b/../c", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/c"));

  ASSERT_EQ(path_normalize("/a/b/../../c", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/c"));

  ASSERT_EQ(path_normalize("/a/b/../", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/"));

  ASSERT_EQ(path_normalize("/a/b/..", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/"));

  ASSERT_EQ(path_normalize("/a/b/..\\", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/"));

  ASSERT_EQ(path_normalize("/a/b\\..", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a/"));

  ASSERT_EQ(path_normalize("\\a\\b/../../c", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/c"));

  ASSERT_EQ(path_normalize("/a/.", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("/a"));

  ASSERT_EQ(path_normalize("F:\\a\\.", result, sizeof(result)), RET_OK);
  ASSERT_EQ(string(result), normalPath("F:/a"));

#ifdef WIN32
  ASSERT_EQ(path_normalize("D:\\aaaa\\aaaaaaaaaaaaaaaaaaaaa\\.\\bin\\demo.exe/../../data/test.c",
                           result, sizeof(result)),
            RET_OK);
  ASSERT_STREQ(result, "D:\\aaaa\\aaaaaaaaaaaaaaaaaaaaa\\data\\test.c");
#endif
}

TEST(Path, build) {
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_build(result, sizeof(result), "a", "b", NULL), RET_OK);
  ASSERT_EQ(string(result), normalPath("a/b"));
}

TEST(Path, abs) {
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_abs("abc", result, sizeof(result)), RET_OK);
  ASSERT_EQ(strstr(result, "abc") != NULL, TRUE);
}

TEST(Path, approot) {
  char result[MAX_PATH + 1];
  char app_root[MAX_PATH + 1];
  memset(app_root, 0x00, sizeof(app_root));
  ASSERT_EQ(path_app_root(app_root), RET_OK);
  ASSERT_EQ(path_build(result, sizeof(result), app_root, "bin", NULL), RET_OK);
  ASSERT_EQ(path_exist(result), TRUE);
}

TEST(Path, approot_ex) {
  char bin_root[MAX_PATH + 1];
  memset(bin_root, 0x00, sizeof(bin_root));
  ASSERT_EQ(path_app_root_ex(bin_root, "bin"), RET_OK);
  log_debug("%s\n", bin_root);
  ASSERT_EQ(path_exist(bin_root), TRUE);
}

static const char* normalize_path_str(char* str) {
  char* p = str;
  while (*p) {
    if (*p == '\\') {
      *p = '/';
    }
    p++;
  }

  return str;
}

TEST(Path, replace_basename) {
  const char* filename = "a/b/test.txt";
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_replace_basename(result, sizeof(result), filename, "abc.txt"), RET_OK);
  ASSERT_STREQ(normalize_path_str(result), "a/b/abc.txt");
}

TEST(Path, replace_extname) {
  const char* filename = "a/b/test.txt";
  char result[MAX_PATH + 1];
  ASSERT_EQ(path_replace_extname(result, sizeof(result), filename, "abc"), RET_OK);
  ASSERT_STREQ(normalize_path_str(result), "a/b/test.abc");
}

TEST(Path, path_remove_last_slash) {
  char result[MAX_PATH + 1];
  strcpy(result, "./a/");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, "./a");

  strcpy(result, "./a\\");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, "./a");

  strcpy(result, "./");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, ".");

  strcpy(result, "/");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, "/");

  strcpy(result, "////");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, "/");

  strcpy(result, "/a/b//");
  ASSERT_EQ(path_remove_last_slash(result), RET_OK);
  ASSERT_STREQ(result, "/a/b");

  ASSERT_NE(path_remove_last_slash(NULL), RET_OK);
}

TEST(Path, path_prepend_app_root) {
  char result[MAX_PATH + 1] = {0};
  ASSERT_EQ(path_prepend_app_root(result, "bin"), result);
  ASSERT_EQ(path_exist(result), TRUE);
}
