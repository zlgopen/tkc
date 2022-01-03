2022/01/03
  * fscript支持保存函数名，方便调试。

2022/01/02
  * 修复emitter被覆盖的问题（感谢智明提供补丁）
  * 修复fscript中while语句对return的处理。

2022/01/01
  * 优化fscript函数调用。
  * 公开结构fscript\_func\_call\_t。
  * fscript 支持设置错误处理函数。
  * fscript 增加函数sleep\_ms。

2021/12/31
  * fscript允许重载print函数。
  * 修复emitter的事件中删除事件同时递归触发事件导致emitter不正常的问题（感谢zxc3489和智明提供补丁）
  * 修复使用thread\_with\_pthread时，线程运行完毕后调 用join函数出现内存泄漏的问题 (感谢雨欣提供补丁)
  * fscript去掉a,b,c,d快速变量，a,b,c,d和普通变量一样处理。

2021/12/29
  * 修复fscript中unset的问题。
  * fscript增加f64/double数据类型转换函数。

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

