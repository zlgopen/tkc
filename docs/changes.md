2022/04/09
  * fscript增加define\_param用于辅助反向生成AWBlock。
  * 完善fscript语法检查

2022/04/07
  * 增加函数url\_to\_string。

2022/04/01
  * 完善URL解析并增加一些新的schema。
  * 完善ubjson writer

2022/03/31
  * 增加带统计功能的stream （感谢林福提供)
  * 增加移除和替换 qaction 的 action\_darray\_thread（感谢林福提供)

2022/03/30
  * fscript 支持延迟解析函数。
  * 完善fscript局部变量的访问。
  * fscript支持通过require函数加载模块。

2022/03/29
  * 优化fscript局部变量的访问。
  * 更新aworkslp移植代码（感谢文静提供)
  * 删除mingw的UNICODE宏和msvc统一宏的版本（感谢智明提供补丁）。

2022/03/28
  * 完善fscript。

2022/03/23
  * 改用mingw默认的宏来定义MINGW宏（感谢智明提供补丁）。
  * 完善ubjson对字符的处理(感谢攀峰提供补丁)

2022/03/22
  * 完善 mingw 编译（感谢智明提供补丁）。

2022/03/21
  * 完善 mingw 编译（感谢智明提供补丁）。

2022/03/20
  * 完善 ubjson\_writer\_write\_object。
  * 完善 fscript 函数len

2022/03/19
  * 完善fscript函数bit\_not
  * 完善fargs\_get\_data\_and\_size
  * 完善object\_date\_time支持foreach。

2022/03/18
  * 完善 tk_\cond\_var\_wait（感谢兆坤提供补丁）。

2022/03/16
  * 增加通配符匹配函数tk\_wild\_card\_match。

2022/03/10
  * 完善fscript debugger

2022/03/04
  * 修改csv中的内存泄漏（感谢林福提供补丁）。

2022/03/02
  * AWorks的 serial\_helper 增加设置超时的接口（感谢林福提供补丁）。

2022/02/24
  * 增加函数str\_format/str\_append\_format。

2022/02/23
  * 修复导出IDL文件的时候因为查找不到依赖的类导致出现警告（感谢智明提供补丁）。
  * 集成了IoT.js，所以同步更新app\_helper\_base.py(感谢朝泽提供补丁)
  * 修复编译警告（感谢智明提供补丁）。

2022/02/22
  * 完善fscript调试器。

2022/02/21
  * 修复fscript global对象初始化的问题。
  * 修复value\_int64/value\_uint64对大数的处理。

2022/02/18
  * 增加函数fscript\_set\_global\_object。

2022/02/15
  * conf对象支持foreach\_prop。
  * conf支持创建子对象。

2022/02/10
  * 完善API注释(感谢朝泽提供补丁)
  * 完善str\_append\_json\_str。

2022/02/04
  * 增加函数path\_basename\_ex。

2022/02/02
  * 完善fscript语法错误检查。

2022/01/29
  * 完善fscript debugger

2022/01/24
  * 优化tk\_strncpy（感谢智明提供补丁）
  * 完善fscript debugger

2022/01/23
  * 完善fscript debugger,支持attach/launch两种启动方式。

2022/01/22
  * 完善fscript debugger

2022/01/21
  * 完善fscript debugger并增加fdb命令行调试工具。
  * 完善aworks平台的serial\_helper.c（感谢文静提供）

2022/01/20
  * 增加fscript的构造函数和析构函数（感谢智明提供补丁）

2022/01/19
  * 完善fscript调试器(支持step in/step out/step over/reload code)。

2022/01/16
  * 完善fscript支持调试器。

2022/01/13
  * 优化tk\_object\_get\_child\_object（感谢雨欣提供补丁）

2022/01/11
  * 修复WITHOUT\_FSCRIPT没有生效的问题。
  * 完善fscript表达式。

2022/01/10
  * 完善fscript if/elseif/else语句
  
2022/01/08
  * fscript的log函数改名为logf，避免与flow中的log重名。
  * fscript增加date\_time\_get\_prop/date\_time\_set\_prop等函数。 

2022/01/07
  * fscript增加str\_is\_empty/str\_len/array\_is\_empty/array\_size等函数。
  * object\_typed\_array支持通过TK\_OBJECT\_PROP\_SIZE获取元素个数。
  * object\_typed\_array支持通过set\_prop/get\_prop访问元素。
  * fscript支持for/for\_in语句。

2022/01/06
  * fscript增加random0to1。
  * fscript支持until/repeat_times/repeat语句。
  * fscript中增加is\_int/is\_odd/is\_even等函数

2022/01/05
  * fscript公开函数value\_id/value\_func。
  * fscript增加log10/pow10等函数。
  * 修复注释错误(感谢朝泽提供补丁)
  * fscript中三角函数增加以度数为单位的相应函数。

2022/01/04
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

