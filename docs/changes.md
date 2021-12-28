2021/12/28
  * 完善serial\_helper.c(感谢朝泽提供补丁)

2021/12/26
  * 增加函数array\_reverse
  * 增加fscript函数array\_reverse

2021/12/25
  * 增加函数str\_reverse/str\_count。
  * 增加fscript函数text\_reverse/text\_count。

2021/12/24
  * 增加状态机(感谢林福提供补丁)
  * 修复单元测试：兼容非utf8编码的编译器（VS2015）(感谢雨欣提供补丁)
  * 完善部分cast函数的文档。

2021/12/23
  * 增加aworkslp平台支持(感谢林福提供补丁)
  * fscript 增加code\_id，方便以后支持调试。
  * 编译嵌入式linux不使用WITH\_SDL宏，以及中文编码问题(感谢陈谭提供补丁)
  * 修正Win32串口操作速度被限制的问题(感谢陈谭提供补丁)

2021/12/22
  * 复原被覆盖的tools/idl\_gen/README.md(感谢雨欣提供补丁)
  * 增加tkc/sha256.h

2021/12/20
  * fscript支持全局对象。

2021/12/11
  * 增加tk\_str\_is\_in\_array。
  * 增加tk\_utf8\_trim\_invalid\_char。

2021/12/09
  * 完善json解析对整数的处理。

2021/12/02
  * TKC 从AWTK中独立出来。
  * 重构编译脚本，增加awtk\_config\_common.py。
  * 一些可能重名函数增加tk_前缀。

2021/12/06
  * 支持linux系统交叉编译(感谢智明提供补丁)。

