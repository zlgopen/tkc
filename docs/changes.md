# 最新动态

2025/08/28
  * 完善tree_node_feature_segment(感谢兆坤提供补丁)

2025/08/23
  * 完善 dlist_remove_ex

2025/08/22
  * 完善tree_node_feature_segment(感谢兆坤提供补丁)
  * 完善注释(感谢兆坤提供补丁)

2025/08/21
  * 添加对path相关函数返回值的处理，避免诸如路径过长导致死循环的问题(感谢朝泽提供补丁)

2025/08/15
  * log_get_log_level/log_get_log_level支持脚本。

2025/08/12
  * improve tree(感谢兆坤提供补丁)

2025/08/11
  * improve tree(感谢兆坤提供补丁)

2025/08/7
  * 增加 线段树结点特征(感谢兆坤提供补丁)

2025/08/5
  * 完善 src/README.md (感谢兆坤发现问题)
  * 完善 utils.c(感谢兆坤提供补丁)
  * 修复 HAS_NO_LIBC 时 缺少 tk_strtoi 实现的问题(感谢兆坤提供补丁)

2025/08/4
  * move object_to_json/object_from_json to conf_utils.h
  * 完善测试(感谢兆坤提供补丁)

2025/08/1
  * 完善 return_ret_if_fail(感谢兆坤提供补丁)
  * 增加 object_from_json(感谢兆坤提供补丁)
  * 增加 darray_revers(感谢兆坤提供补丁)

2025/07/29
  * 修复由于没有正确返回返回值导致诸如"&q"表达式会死循环的问题(感谢朝泽提供补丁)

2025/07/26
  * 完善darray/dlist

2025/07/24
  * 删除多余的path_extname_is原型声明(感谢兆坤发现问题)
  * 修复api comment

2025/07/23
  * 优化了linux和macos系统中awtk程序搜索动态库的路径，程序加载时会自动搜索同目录的动态库文件 (感谢陈谭提供补丁)
  * 增加 path_extname_is_one_of

2025/07/21
  * 增加 return_ret_if_fail
  * 增加 tkc/tools/idl_gen/node_modules (感谢陈谭提供建议)
  * 完善编译脚本并同步AWTK文件 (感谢陈谭提供补丁)
  * 增加 tk_str_trim_right/tk_str_trim_left

2025/07/18
  * slist_remove_ex 替换 slist_remove_with_compare (感谢兆坤提供补丁)

2025/07/15
  * 增加文档[如何让 json 文件和 object 之间相互转换](how_to_convert_between_json_file_and_object.md) (感谢兆坤提供补丁)。

2025/07/09
  * fscript 支持 obj.exec/obj.exec_ex

2025/07/07
  * 增加函数 value_compare (感谢兆坤提供补丁)。 

2025/07/03
  * 修复awtk-nodejs编译失败的问题(感谢俊杰提供补丁) 

2025/07/02
  * 完善 tree_to_string (感谢兆坤提供补丁)。
  * 完善 tree_to_string2 (感谢兆坤提供补丁)。

2025/07/01
  * 增加函数 dlist_get
  * 增加函数 slist_get

2025/06/30
  * set shared node allocator(感谢兆坤提供补丁)。

2025/06/27
  * add tree_node_get_lowest_common_ancestor (感谢兆坤提供补丁)。

2025/06/26
  * 完善 conf_ubjson (感谢林福提供补丁)
  * 修复拼写错误(感谢兆坤提供补丁)。

2025/06/25
  * 完善 typed_array (感谢兆坤提供补丁)。

2025/06/24
  * 完善 typed_array (感谢兆坤提供补丁)。

2025/06/19
  * 修改mingw编译时动态库按def文件导出符号(感谢朝泽提供补丁)

2025/06/16
  * 完善 action thread 支持指定超时参数。

2025/06/14
  * 完善 宏定义(感谢兆坤提供补丁)。

2025/06/13
  * 增加 tk_isalnum

2025/06/09
  * 完善 tkc.h(感谢兆坤提供补丁)。

2025/06/07
  * 增加 fscript_create_ex2(修复awtk-mvvm data binding出现循环引用计数的问题)
  * 增加 tk_object_life_t

2025/06/06
  * 完善 conf_ubjson (感谢林福提供补丁)

2025/05/30
  * 修复增加USE_TK_PREFIX宏后导致编译不过的问题(感谢智明提供补丁)

2025/05/28
  * 完善 tk_object_exec_ex(感谢兆坤提供补丁)。
  * 增加了对OpenRTOS（FreeRTOS的商业版）的头文件包含，并修正线程退出时自动释放资源（感谢陈谭提供补丁)。

2025/05/23
  * 完善 value_bool

2025/05/22
  * 修复pthread_cond_timedwait返回EINVAL的问题(感谢智明提供补丁)

2025/05/19
  * 完善fscript对象生命周期管理(感谢兆坤提供补丁)。

2025/05/16
  * 修复了str追加中文之后size错误的问题(感谢陈聪提供补丁)

2025/05/13
  * 完善宏定义(感谢兆坤提供补丁)。
  * 增加fs_foreach_dir(感谢林福提供补丁)

2025/05/12
  * 完善宏定义(感谢兆坤提供补丁)。

2025/05/10
  * fscript增加exec_ex (感谢兆坤提供补丁)。

2025/05/09
  * 编译mingw的时候读取网卡ip的函数编译错误(感谢智明提供补丁)
  * 增加 tk_object_exec_ex 函数(感谢兆坤提供补丁)

2025/05/08
  * 完善typed_array
  * 完善宏定义(感谢兆坤提供补丁)。

2025/05/07
  * 增加函数 fscript_set_name
  * 完善mem_allocator(感谢兆坤提供补丁)。

2025/05/06
  * object hash 兼容32位系统(感谢兆坤提供补丁)。
  * 完善tk_watob，与tk_atob逻辑相同(感谢兆坤提供补丁)。
  * 增加根据网卡名字获取ip的函数和增加listen_ex可以增加ip参数(感谢智明提供补丁)

2025/04/30
  * 完善 mem_allocator_fixed_block(感谢兆坤提供补丁)。
  * 完善fscript对 % 的处理(感谢福明发现问题)

2025/04/29
  * 完善 mem_allocator_fixed_block(感谢兆坤提供补丁)。

2025/04/28
  * mem_allocator_fixed_block 隔离管理链表与用户内存 (感谢兆坤提供补丁)。

2025/04/27
  * 增加value_replace，用于避免value\ data的内存地址相同导致先reset后deep_copy出现野指针的问题(感谢兆坤提供补丁)。

2025/04/25
  * object有属性删除时发送EVT_PROPS_CHANGED事件(感谢兆坤提供补丁)。 

2025/04/23
  * fscript 支持 %开头的变量名。

2025/04/16
  * 完善tk_swap(感谢兆坤提供补丁)。
  * 完善xml对pi的解析(感谢朝泽提供补丁)

2025/04/15
  * 增加mem_allocator_fixed_block，dlist/slist/tree使用该内存分配器管理内存(感谢兆坤提供补丁)。
  * 完善mem_allocator_fixed_block(感谢兆坤提供补丁)。

2025/04/14
  * conf_io 支持 YAML格式。

2025/04/13
  * 完善slist/dlist(感谢兆坤提供补丁)

2025/04/09
  * 完善object_hash(感谢兆坤提供补丁)
  * 修复拼写错误(感谢兆坤提供补丁)

2025/04/03
  * 完善tree(感谢兆坤提供补丁)

2025/04/02
  * 完善tree测试程序(感谢兆坤提供补丁)
  * 完善tree_to_string(感谢兆坤提供补丁)
  * 修复tree内存泄露(感谢兆坤提供补丁)

2025/04/01
  * 增加tree_to_string(感谢兆坤提供补丁)

2025/03/30
  * 完善tree(感谢兆坤提供补丁)

2025/03/29
  * 增加函数 tk_rad_equal/tk_normalize_rad

2025/03/28
  * 完善树结构(感谢兆坤提供补丁)

2025/03/27
  * 增加树结构(感谢兆坤提供补丁)

2025/03/24
  * 增加静态断言(感谢兆坤提供补丁)

2025/03/05
  * 去除函数定义了没使用的警告(感谢兆坤提供补丁) 

2025/03/04
  * 完善atomic(感谢兆坤提供补丁)
  * 修复 atomic 在 c 上编译报错的问题(感谢兆坤提供补丁)

2025/02/27
  * 增加event_source_fd对象支持fd为-1和设置fd的情况(感谢智明提供补丁)

2025/02/26
  * 修复conf_node的node_type不正确的问题(感谢福明提供补丁)

2025/02/22
  * 增加函数 waitable_ring_buffer_is_empty

2025/02/18
  * 增加函数 tk_cond_var_clear

2025/02/17
  * 完善 conf_ubjson (感谢林福提供补丁)

2025/02/14
  * 实现了 ubjson 规范中的使用类型和计数进行数组优化(感谢林福提供补丁)

2025/02/09
  * 完善matrix注释(感谢兆坤提供补丁)

2025/02/08
  * 增加wbuffer_modify_binary等函数(感谢林福提供补丁)

2025/01/28
  * 补充注释(感谢兆坤提供补丁) 
  * 修复仿射变换顺序错误(感谢兆坤提供补丁)

2025/01/21
  * 完善 freertos 移植。
  * 增加str追加wchar的函数(感谢智明提供补丁)

2025/01/01
  * 完善 freertos 移植。

2024/12/08
  * 增加wstr\_modify\_wchar(感谢智明提供补丁)

2024/12/02
  * ubjson支持传输指针类型(感谢智明提供补丁)
  * 增加弃用编译警告(感谢兆坤提供补丁)
  * 修复文档拼写错误(感谢兆坤提供补丁)

2024/11/21
  * 完善conf_json(感谢兆坤提供补丁)

2024/11/19
  * 增加函数 str_append_format_padding
  * 增加 atomic_compare_exchange(感谢兆坤提供补丁)
  * 完善 atomic(感谢兆坤提供补丁)

2024/11/18
  * 完善 atomic(感谢兆坤提供补丁)
  * 修复拼写错误(感谢兆坤提供补丁)

2024/11/17
  * 完善 TK_STRINGIZE(感谢兆坤提供补丁)
  * value完善调试log(感谢兆坤提供补丁)
  * 修复拼写错误(感谢兆坤提供补丁)
  * 增加原子操作(感谢兆坤提供补丁)

2024/11/13
  * 增加 TK_STRINGIZE(感谢兆坤提供补丁)

2024/11/08
  * 完善object_array 和 tk_str_indexable(感谢兆坤提供补丁)

2024/11/07
  * 完善object_hash(感谢兆坤提供补丁)
  * 完善object_array(感谢兆坤提供补丁)
  * 完善tk_atob(感谢兆坤提供补丁)
  * 统一是否可从名字中获取index的判断逻辑(感谢兆坤提供补丁)

2024/11/06
  * object hash 增加 keep_props_order 属性(感谢兆坤提供补丁)
  * 完善 named_value_hash(感谢兆坤提供补丁)

2024/11/04
  * 完善 object_array 和 named_value(感谢兆坤提供补丁)

2024/10/30
  * 完善object\ compare(感谢兆坤提供补丁) 

2024/10/28
  * 修改编译警告(感谢兆坤提供补丁)
  * 完善tokenizer_init_ex的错误处理。

2024/10/24
  * tk_swap防止临时变量重名(感谢兆坤提供补丁)

2024/10/23
  * 增加tk_sha256_hash_from_str(感谢兆坤提供补丁)
  * 修改object_to_json和tk_object_to_json重复的问题。

2024/10/21
  * 完善object to json(感谢兆坤提供补丁)

2024/10/17
  * 完善fscript错误提示(感谢兆坤提供补丁)

2024/10/15
  * 修复拼写错误(感谢兆坤提供补丁)
  * 完善csv (感谢兆坤提供补丁)
  * 修复value_equal(感谢兆坤提供补丁)

2024/10/14
  * 完善object\ array复制全部的属性的功能(感谢兆坤提供补丁)

2024/10/12
  * 还原tk_strcpy修改(感谢兆坤提供补丁)
  * 兼容拼写错误处理(感谢兆坤提供补丁)
  * 修复编译警告(感谢兆坤提供补丁)

2024/10/11
  * 统一date_time_format处理(感谢兆坤提供补丁)

2024/10/10
  * 完善object compare(感谢朝泽提供补丁)
  * 修复拼写错误(感谢兆坤提供补丁)

2024/10/09
  * 补充fscript函数(感谢兆坤提供补丁)

2024/10/08
  * 完善链表(感谢兆坤提供补丁)
  * 修复 conf_json_parse (感谢兆坤提供补丁)
  * 完善宏定义(感谢兆坤提供补丁)

2024/10/01
  * 完善链表(感谢兆坤提供补丁)
  * 修复tk_str_copy内存重叠问题(感谢兆坤提供补丁)

2024/09/30
  * 增加双向链表(感谢兆坤提供)

2024/09/24
  * 拓展类型开启后，修复无法设置空的object的问题(感谢兆坤提供补丁)

2024/09/19
  * 修复时间判断条件不正确的问题(感谢智明提供补丁)

2024/09/18
  * 修复 ubjson_writer 对wstr的处理(感谢兆坤提供补丁)

2024/09/17
  * app_conf初始化时，不拷贝默认配置到用户配置，而是优先读取用户配置，再读取默认配置。

2024/09/07
  * 增加ubjson_writer整型接口(感谢智明提供补丁)

2024/09/05
  * 完善部分变量定义方便调试(感谢兆坤提供补丁)
  * 修复object_find不正常的问题(感谢智明提供补丁)

2024/09/04
  * 修复tk_strncpy_s用法(感谢颖健提供补丁)

2024/08/29
  * 修复命令行参数短名和长名比较导致误判的情况(感谢智明提供补丁)

2024/08/26
 * 简化named\_value\_hash函数，去除进制设置(感谢兆坤提供补丁)
 * 修复named\_value\_hash\_compare比较大小错误的问题(感谢兆坤提供补丁)
 * ubjson支持u16、u32、u64类型(感谢兆坤提供补丁)

2024/08/23
  * 增加 clear\_props、find\_prop、find\_props接口(感谢兆坤提供补丁)

2024/08/21
  * darray去除告警信息(感谢兆坤提供补丁)
  * 保持object_default_remove_prop\ 与\ set_prop\ get_prop\ 逻辑一致(感谢兆坤提供补丁)
  * 修正 object_default_test(感谢兆坤提供补丁)
  * 增加 object_hash/named_value_hash(感谢兆坤提供补丁)

2024/08/15
  * 修改debugger_lldb_get_callstack_ex增加tid的参数并且增加fdb的测试例子(感谢智明提供补丁)

2024/08/12
  * debugger增加一个获取指定层数堆栈的api(感谢智明提供补丁)

2024/08/11
  * 增加函数 tk_mergesort

2024/08/9
  * 增加 goto_error_if_fail_ex(感谢兆坤提供补丁)

2024/08/8
  * 增加 tk_swap(感谢兆坤提供补丁)

2024/08/2
  * 增加判断当前停在哪个线程的逻辑(感谢朝泽提供补丁)

2024/08/1
  * 修复了object_default读写 "[a]" 这样的属性名会出错的问题（感谢陈聪提供补丁）

2024/07/31
  * 修改app_helper没有从compile config中获取LCD_ORIENTATION的问题(感谢朝泽提供补丁)
  * 去除 tk_free_impl 告警信息(感谢兆坤提供补丁)

2024/07/23
  * 优化conf_json_parse_array接口效率(感谢雨欣提供补丁)
  * 修复部分linux设备获取串口配置不正常的问题(感谢智明提供补丁) 

2024/07/22
  * 修改app_helper优先使用compile config设定的APP_ROOT(感谢朝泽提供补丁)

2024/07/19
  * 支持lldb正常退出后不会暂停runtime程序(感谢智明提供补丁)
  * 修正awtk的scons脚本在scons4.8版本运行出错的问题(感谢陈谭提供补丁)

2024/07/18
  * 完善注释(感谢雨欣提供补丁)
  * 修复tk_angle接口单元测试不通过的问题(感谢雨欣提供补丁)

2024/07/09
  * 增加函数 tk\_socket\_get\_port (感谢智明提供补丁)

2024/07/04
  * 完善 socket_helper.c，允许listen端口为0，系统自动分配端口。

2024/06/24
  * fscript支持注册自定义事件(感谢雨欣提供补丁)

2024/06/19
  * 增加函数tk\_str\_case\_start\_with/tk\_str\_case\_end\_with

2024/06/3
  * 修复lldb切换线程时候不会自动获取新的线程堆栈信息的问题(感谢智明提供补丁)

2024/05/29
  * 修复debugger_lldb获取局部变量和全局变量的函数都是获取全部变量的问题(感谢智明提供补丁)

2024/05/28
  * event\_source\_manager支持微秒级的sleep。
  * 增加sleep\_us(目前只支持linux)

2024/05/24
  * 增加函数tk\_int\_is\_in\_array

2024/05/8
  * 完善value_uint32(感谢华健提供补丁)
  * 完善fscript iformat(感谢华健提供补丁)
  * 完善debugger_lldb(感谢智明提供补丁)

2024/04/19
  * 增加函数str\_shrink/wstr\_shrink。

2024/04/09
  * conf doc extend type\ 支持数组类型(感谢兆坤提供补丁)

2024/04/08
  * 执行前根据预解析生成的symbols提前做好locals对象中的占位变量，避免出现解析id与局部变量对不上的情况(感谢雨欣提供补丁)
2024/04/01
  * 增加 debugger 的中断事件对象的函数名字属性(感谢智明提供补丁)
  * 完善 debugger_lldb 的timeout，支持从配置文件读取。

2024/03/28
  * object array vtable增加clone函数(感谢兆坤提供补丁)

2024/03/27
  * 更新部分文件的copyright日期(感谢俊圣提供补丁)。
  * 增加函数 tk\_object\_is\_instance\_of

2024/03/23
  * 增加函数 conf_node_get_xxx

2024/03/21
  * 完善tk\_buffer\_set\_value/tk\_buffer\_get\_value
  * fscript 支持用+拼接binary\_data

2024/03/20
  * 增加函数tk\_object\_set\_prop\_str\_with\_format

2024/03/16
  * 完善debugger\_lldb修改请求超时时间。
  * 完善debugger\_lldb支持targetCreateCommands

2024/03/15
  * 修改内存泄露。
  * 增加函数tk\_distance/tk\_value\_to\_angle/tk\_angle

2024/03/14
  * 补充调试信息(感谢兆坤提供补丁)

2024/03/12
  * 完善函数 tk\_buffer\_set\_value/tk\_buffer\_get\_value
  * 增加函数 tk\_size\_of\_basic\_type

2024/03/10
  * 增加函数tk\_basic\_type\_from\_name

2024/03/08
  * 增加函数 tk\_buffer\_set\_value/tk\_buffer\_get\_value

2024/03/07
  * debuger支持load config，用于让lldb加载init命令。
  * 导出 xml\_file\_expand\_subfilenames\_get(感谢颖健提供补丁)

2024/03/06
  * 增加函数tk\_tcp\_is\_port\_in\_use

2024/03/04
  * 增加zip_file。

2024/02/23
  * 增加了tk\_tcp\_connect\_ex函数，给tk\_stream\_factory\_create\_iostream函数增加了设置连接超时的timeout参数支持，主要是让awtk-hmi的modbus主站可以设置超时参数，比如url":"tcp://localhost:502?timeout=3000(感谢陈谭提供补丁)
  * xml\_file\_expand 函数去除 property 标签作用域下的屏蔽(感谢高源提供补丁)

2024/02/21
  * fscript增加 can_exec (感谢兆坤提供补丁) 

2024/02/20
  * 修复拼写错误(感谢兆坤提供补丁)
  * 修复android编译警告。

2024/02/19
  * 增加函数str\_append\_json\_pair（感谢林福提供补丁)

2024/02/07
  * 增加函数tk\_bits\_data\_to\_bytes\_data/tk\_bits\_data\_from\_bytes\_data

2024/01/23
  * 修复pthread无法修改优先级和栈大小的问题(感谢智明提供补丁)

2024/01/21
  * add tk\_pointer\_to\_long/tk\_pointer\_from\_long/tk\_bits\_to\_bytes

2024/01/15
  * 修复mingw编译app的时候报错的问题(感谢智明提供补丁) 

2024/01/13
  * 增加一个编译参数类型用于None的情况下识别编译传参类型以及修复app和awtk编译模式不同的问题(感谢智明提供补丁)
  * 兼容旧的mac系统的sdl路径(感谢智明提供补丁)

2024/01/12
  * 实现 conf\_doc\_foreach 函数(感谢林福提供补丁)
  * 实现 data\_reader\_can\_read  函数(感谢林福提供补丁)
  
2024/01/11
  * 增加一个编译参数类型用于None的情况下识别编译传参类型以及修复app和awtk编译模式不同的问题(感谢智明提供补丁)

2024/01/09
  * 完善 utils.py，兼容Python2.7(感谢雨欣提供补丁）
  * 完善 url_parse(感谢志鹏提供补丁)

2024/01/05
  * 编译脚本scons 版本不一致判断函数漏改问题(感谢高源提供补丁)。

2024/01/02
  * 增加函数 tokenizer\_next\_str\_until
  * 更新软件版权日期。

2023/12/30
  * 完善log hook方便支持多线程。
  * 为了避免冲突将tkc中的log\_level\_t改为tk\_log\_level\_t 

2023/12/27
  * 增加函数tk\_islower/tk\_isupper(感谢兆坤提供补丁)
  * 增加函数slist\_find\_ex(感谢兆坤提供补丁)

2023/12/26
  * 完善API注释错误（感谢俊杰发现问题)
  
2023/12/25
  * 完善内存管理，支持直接使用OS提供的函数。

2023/12/24
  * 完善data reader
  * data\_reader\_http 支持 chunked data 
  * CSV 支持设置列名，通过列名获取数据。

2023/12/23
  * 增加函数path\_expand\_vars

2023/12/22
  * 增加data\_reader\_http。

2023/12/19
  * 修复API注释(感谢俊杰提供补丁)

2023/12/18
  * 修复lldb获取变量的时候返回为空值的问(感谢智明提供补丁)

2023/12/15
  * 完善csv file object

2023/12/13
  * fscript 增加函数sha256。

2023/12/06
  * fscript 增加 date\_time\_format
  * 修复传入命令行编译脚本不正常的问题(感谢智明提供补丁)

2023/12/05
  * 完善fscript部分数学函数。
  
2023/12/04
  * 增加函数tk_levelize。fscript增加levelize函数。
  * 修改debugger\_lldb\_stop等待超时的问题以及修复continue显示问题 (感谢智明提供补丁)

2023/12/01
  * conf_json 支持块注释/**/和行注释//
  * 修复 iostream_serial.c 交叉编译问题(感谢明浩提供补丁)

2023/11/28
  * 完善log hook

2023/11/26
  * 增加函数 wbuffer\_write\_value/rbuffer\_read\_value
  * 完善 ubjson\_writer

2023/11/24
  * 修改 debugger 的 dispatch_messages 函数和调试下一步等按钮会修改 debugger 状态(感谢智明提供补丁)

2023/11/23
  * 修正awtk的scons参数args的统一处理，以及删掉一些重复写的编译参数。对scons参数为none时才设置为默认值，对参数值为空字符串及空数组时直接替换进去不做特殊处理。(感谢陈谭和智明提供补丁)

2023/11/17
  * 增加函数action\_darray\_thread\_set\_idle\_interval((感谢林福提供补丁)
  * 修复Windows下打开串口失败tk\_iostream\_serial\_create崩溃的问题。

2023/11/16
  * 修复path\_basename\_ex 可能非法访问内存的问题(感谢俊圣提供补丁)

2023/11/14
  * 导出darray\_bsearch\_index\_ex接口(感谢雨欣提供补丁)

2023/11/11
  * 增加函数fs\_file\_equal

2023/11/9
  * 完善serial helper
  * 修复value_int64接口强转uin32类型值对象时出现溢出的问题(感谢雨欣提供补丁)

2023/11/8
  * 关闭lldb的打印调试信息(感谢智明提供补丁)
  * 统一处理提取路径文件名的处理函数(感谢智明提供补丁)

2023/11/7
  * socket初始化时忽略SIGPIPE。
  * 增加了get和set当前线程id的函数以及修改获取堆栈的返回值类型(感谢智明提供补丁)

2023/11/03
  * 修复子进程继承句柄机制有概率导致父进程异常的问题(感谢智明提供补丁)

2023/10/30
  * 修复conf\_doc\_dup\_node复制数组的问题(感谢俊杰提供补丁)。
  * 增加debugger\_get\_curr\_frame\_index函数(感谢智明提供补丁)
  * 修复tk\_wstr\_select\_word编译警告(感谢颖健提供补丁)

2023/10/27
  * debugger的中断事件和堆栈改变事件增加文件路径(感谢智明提供补丁)

2023/10/25
  * 增加函数tk\_iostream\_serial\_create\_ex
  * 修复编译报错的问题(感谢智明提供补丁)
  * WASM 实现vsscanf/sscanf
  * 修复没有导出debugger\_launch\_app的问题(感谢智明提供补丁)

2023/10/24
  * 完善stream。
  * 修复子进程断开信号不正常的问题以及修改了测试用例(感谢智明提供补丁)

2023/10/23
  * 增加函数wbuffer\_create/wbuffer\_destroy/rbuffer\_create/rbuffer\_destroy
  * 增加杀死子进程的函数和设置子程序工作目录函数(感谢智明提供补丁)
  * 修复子进程断开信号不正常的问题(感谢智明提供补丁)

2023/10/20
  * 完善conf\_node\_set\_value

2023/10/19
  * 增加函数fs\_stat\_info\_create/fs\_stat\_info\_destroy
  * 增加函数fs\_item\_create/fs\_item\_destroy

2023/10/18
  * 修复process完成后读写崩溃的问题以及修改linux下全双工的管道问题(感谢智明提供补丁)

2023/10/16
  * 增加函数 conf\_node\_get\_child\_value\_int32/conf\_node\_get\_child\_value\_bool/conf\_node\_get\_child\_value\_str

2023/10/12
  * 增加process支持使用命令行启动子程序以及修复编译警告(感谢智明提供补丁)

2023/10/11
  * 修改WITH_JERRYSCRIPT的值默认为false(感谢智明提供补丁)

2023/10/09
  * 修复value\_test中的警告信息(感谢高源提供补丁)

2023/10/07
  * 增加子程序的模块(感谢智明提供补丁)

2023/10/04
  * 增加RET\_EXCEED\_RANGE

2023/10/03
  * 增加RET\_INVALID\_ADDR

2023/10/02
  * 增加函数tkc\_crc16\_modbus
  * 增加stream\_factory

2023/10/01
  * 增加函数url\_get\_param\_int32/url\_get\_param\_bool

2023/09/25
  * 按标准c库修正iswspace 函数定义(感谢陈谭提供补丁)
  * 增加函数csv\_file\_object\_load/csv\_file\_object\_load\_from\_buff
  * 增加函数csv\_file\_object\_save\_to\_buff/csv\_file\_object\_save\_as
  * xml/json/ubjson/ini/hash_table增加示例代码。

2023/09/25
  * 增加函数path\_prepend\_temp\_path
  * 增加函数path\_prepend\_user\_storage\_path

2023/09/24
  * 增加tk\_iostream\_tcp\_create\_client
  * 增加函数hash\_table\_size

2023/09/23
  * 完善socket\_helper
  * 完善serial\_helper
  * 将serial\_helper.c/.h移动到tkc目录。

2023/09/21
  * 完善serial\_open的注释(感谢俊杰提供补丁)
  * 兼容旧的liunx-fb编译报错的问题(感谢智明提供补丁)

2023/09/20
  * 修改fs_stat对mingw的宽字符串问题(感谢智明提供补丁)
  * conf_doc增加一个标志位控制是否可以get/set object类型(感谢兆坤提供补丁)。
  * 完善WASM支持。

2023/09/18
  * 修复命令行使用mwindow不正常的问题(感谢智明提供补丁)

2023/09/15
  * 修复 path\_basename\_ex 的对于混合斜杆不支持的问题(感谢智明提供补丁)
  * 增加scons清除的时候顺便把lib和bin目录也删除功能(感谢智明提供补丁)

2023/09/13
  * 修复当BUILD_DIR指定的路径在SConstruct文件所在目录之外的目录时无法正常编译的问题(感谢培煌提供补丁)

2023/09/08
  * 给APP的编译参数增加WIN32_RES参数(感谢智明提供补丁)

2023/09/07
  * 统一宏TK\_STRDUP和TK\_STRNDUP与对应接口的行为(感谢雨欣提供补丁)

2023/09/04
  * conf\_node支持object类型(感谢兆坤提供补丁)

2023/08/30
  * 增加函数tk\_sscanf\_simple
  * 完善编译(感谢智明提供补丁)

2023/08/28
  * 增加函数tk\_socket\_get\_client\_ip/tk\_socket\_get\_self\_ip/tk\_socket\_get\_client\_ip\_str/tk\_socket\_get\_self\_ip\_str
  * tcp\_send/tcp\_recv增加命令行参数。
  * 增加函数tk\_ostream\_write\_str/tk\_ostream\_printf
  * 增加函数path\_abs\_normalize/path\_abs\_normalize\_with\_root

2023/08/25
  * 完善wasm helper
  * 完善编译(感谢智明提供补丁)

2023/08/24
  * 完善wasm helper
  * 修复字符串为中文的时候判断是否为空格时候会断言问(感谢智明提供补丁)

2023/08/23
  * 让部分函数支持scriptable属性(感谢兆坤提供补丁)
  * 完善编译脚本(感谢高源提供补丁)

2023/08/21
  * 增加函数cmd\_args\_process\_wstr
  * 增加函数tk\_utf8\_dup\_wstr
  * 增加函数tk\_to\_utf8\_argv
  * 增加函数tk\_free\_utf8\_argv
  * 增加宏MAIN/END\_MAIN
  * 完善Windows平台初始化，让Console输出支持UTF8。
   

2023/08/16
  * 增加函数str\_equal/wstr\_eq/wstr\_create/wstr\_destroy/wstr\_append\_more等函数
  * 增加宏STR\_DESTROY/WSTR\_DESTROY。

2023/08/10
  * 完善 event\_source\_manager\_default 让调用者有机会知道 select 是否失败(感谢林福提供补丁)。
  * 完善conf json支持按指定的方式缩进(感谢林福提供补丁)。

2023/08/09
  * 增加函数bits\_stream\_set/bits\_stream\_get

2023/08/08
  * 完善str\_replace(感谢陈聪提供补丁)

2023/08/01
  * 将tyeps\_def.h中WASM相关的代码移到wasm\_helper.h中。

2023/07/27
  * 增加ROMFS。

2023/07/26
  * 修改Windows下路径带中文时fs\_os\_get\_exe的返回结果不是utf8的问题(感谢朝泽提供补丁)

2023/07/19
  * 完善str/wstr注释(感谢智明提供补丁)

2023/07/13
  * 完善conf\_doc\_create分配内存失败的处理(感谢林福提供补丁)。

2023/07/05
  * 完善serial helper的文档(感谢俊杰提供补丁)。
  * 完善ubjson\_writer\_write\_kv\_value。

2023/07/04
  * 完善conf\_node\_get\_child\_value。

2023/07/03
  * 新增str\_append\_uint32(感谢陈聪提供补丁)
  * 修复json写uint32的问(感谢陈聪提供补丁)

2023/06/30
  * 增加函数conf\_ini\_load\_from\_buff/conf\_ini\_save\_to\_buff
  * 增加函数conf\_json\_load\_from\_buff/conf\_json\_save\_to\_buff
  * 增加函数conf\_xml\_load\_from\_buff/conf\_xml\_save\_to\_buff
  * 增加函数conf\_ubjson\_load\_from\_buff/conf\_ubjson\_save\_to\_buff

2023/06/28
  * 感谢朝泽提供以下补丁：
    * 修正 timegm 在 windows 下不识别的问题；
    * 修正 '\e' 在 windows 下为不可识别的字符转义序列的问题；
  * 修复API注释错误(感谢俊圣提供补丁)

2023/06/26
  * 完善date\_time\_to\_time/date\_time\_from\_time：
    * 类型由uint64改为int64。
    * 本地时间改为GMT时间。

2023/06/22
  * 增加函数ret\_code\_from\_name/ret\_code\_to\_name。

2023/06/21
  * 修复action\_thread quit时间过长问题(感谢兆坤提供补丁)。
  * 增加函数slist\_tail/slist\_head。

2023/06/17
  * 增加函数event\_source\_manager\_set\_min\_sleep\_time。

2023/06/09
  * 修复子项目打包时python版本不一致导致的protocol:4/5问题(感谢高源提供补丁);

2023/06/06
  * object\_array\_clean\_invalid\_props在没有清理非法属性时，不分发ITEMS\_CHANGED事件。
  * UBJSON扩展支持WSTRING。

2023/05/31
  * 增加tk\_lltoa等函数(感谢智明提供补丁) 

2023/05/29
  * 完善注释(感谢泽武提供补丁)

2023/05/28
  * 增加函数emitter\_forward\_retarget。

2023/05/27
  * 完善ubjson\_do\_parse\_array数组中包含数组的处理。

2023/05/26
  * 增加函数ubjson\_writer\_write\_kv\_array\_begin。
  * 增加函数conf\_node\_get\_child\_value。
  * 增加函数conf\_node\_get\_child\_value\_by\_index。
  * 修复fs\_file\_read\_line函数在读取到空行时文件指针异常的问题(感谢泽武提供补丁)

2023/05/19
  * 增加了debugger的get和set调试状态的重载函数指针(感谢智明提供补丁)
  * 让data\_writer\_factory与data\_reader\_factory逻辑一致(感谢兆坤提供补丁)

2023/05/16
  * 增加函数rect\_diff(感谢智明提供补丁)
  * 增加函数slist\_is\_empty(感谢智明提供补丁)

2023/05/13
  * 扩展debugger接口支持VSCode DAP协议。
  * 增加函数value\_type\_name。

2023/05/04
  * 完善tk\_object\_to\_json对不同数据类型和数组的处理。

2023/04/26
  * 修复object\_date\_time\_test.cc中的错误(感谢[szsam](https://github.com/szsam)提供补丁)

2023/04/22
  * 增加函数str\_append\_unescape/str\_append\_escape/str\_append\_c\_str。

2023/04/21
  * 完善action thread pool(感谢俊杰提供补丁)
  * 完善文档注释(感谢俊杰提供补丁)

2023/04/18
  * 修复thread\_with\_sdl的内存泄露的问题(感谢智明提供补丁)
  
2023/04/15
  * 增加cmd_args用于解析命令行参数。

2023/04/14
  * 优化XML写入(感谢陈聪提供补丁)

2023/04/12
  * 增加函数file\_read\_as\_unix\_text。
  * 增加函数str\_attach\_with\_size。
  * 优化str\_replace。

2023/04/06
  * 增加函数str\_attach/wstr\_attach/wstr\_append\_int。

2023/03/27
  * 完善api\_doc\_lint工具(感谢俊圣提供补丁)  

2023/03/16
  * 完善JSON((感谢俊杰提供补丁)

2023/03/15
  * 完善types\_def.h(感谢雨欣提供补丁)

2023/03/14
  * easing 完善注释与单元测试(感谢泽武提供补丁)

2023/03/09
  * 新增scons dblite版本检查，编译时python版本与当前python版本不一致则删除dblite(感谢高源提供补丁)

2023/03/08
  * 修改编译警告(感谢智明提供补丁)。

2023/03/07
  * 添加注册easing接口(感谢泽武提供补丁)

2023/03/03
  * 增加ASSERT\_STREQ\_UNIX(gtest\_helper.h)用于在单元测试中跨平台比较从文件中读取的字符串。

2023/03/01
  * 完善debugger接口(感谢智明提供补丁)。

2023/02/24
  * 修复API注释错误(感谢俊圣提供补丁)

2023/02/23
  * 修复API注释错误(感谢俊圣提供补丁)

2023/02/22
  * 修复API注释错误(感谢雨欣提供补丁)
  * 修改path_normalize的测试用例不兼容linux的问题(感谢智明提供补丁)

2023/02/21
  * 修复API注释错误(感谢俊圣提供补丁)
  * 修改path\_normalize对末尾为点的处理问题(感谢俊圣提供补丁)

2023/02/20
  * 修复API注释错误(感谢俊圣提供补丁)

2023/02/19
  * 增加函数tk\_sha256\_file。
  * 增加函数tk\_crc32\_file。

2023/02/17
  * 修复API注释错误(感谢俊圣提供补丁)
  * str\_t类型新增create和destroy接口(感谢雨欣提供补丁)

2023/02/16
  * 修复API注释错误(感谢俊圣提供补丁)
  
2023/02/14
  * 修复fscript注释错误(感谢俊圣提供补丁)

2023/02/03
  * 去除conf\_obj\_create时不必要的警告信息(感谢雨欣提供补丁)

2023/01/29
  * 修复emitter多层消息分发递归后释放最顶级的iter后出现野指针导致崩溃的问题(感谢智明提供补丁)

2023/01/13
  * 修改asset\_info\_t兼容以前的资源以及修复不同平台下asset\_info\_t大小不 同的问题(感谢智明提供补丁)
  * 修改asset_info的name属性的注释(感谢智明提供补丁)

2023/01/12
  * 完善注释(感谢泽武提供补丁)

2023/01/10
  * 修正一个scons兼容性的问题，主要是如果windows系统是64bit，但安装的python是32bit的情况下，原来的scons脚本会出错(感谢陈谭提供补丁)。
  * 修正mingw中没有wcscasecmp函数编译出错的问题(感谢陈谭提供补丁)。

2023/01/05
  * 完善编译脚本(感谢雨欣提供补丁)

2023/01/04
  * 修复了fs\_create\_dir\_r函数中tokenizer没有释放就return的问题(感谢泽武提供补丁)
  * 完善了 awtk 的编译脚本，在 win 平台上，默认指定 scons 使用 msvc 编译器，除非用户自己指定 TOOLS_NAME='mingw'，避免按照 scons 内部的顺序来查找编译器(感谢雨欣提供补丁)
  * 完善app\_helper\_base(感谢雨欣提供补丁)

2023/01/03
  * 增加函数value\_dup\_wstr(感谢雨欣提供补丁)
  * 资源名称长度超过上限时，另外申请空间来保存(感谢陈聪提供补丁)

2022/12/28
  * fscript 不再管obj的引用计数(感谢兆坤提供补丁)

2022/12/22
  * 公开函数 conf\_node\_find\_child\_by\_index(感谢俊杰提供补丁)

2022/12/07
  * fscript 支持省略参数。如:print(1,,2)，方便函数内部使用默认值。
  
2022/12/03
  * fscript 支持注册常量。

2022/11/25
  * 修复当控件头文件中存在多个类的时候导致虚表函数导出失败的问(感谢智明提供补丁)

2022/11/18
  * 完善log系统，支持调试器拦截日志。
  * 完善了 conf\_obj\_create 接口，避免打印不必要的警告信息(感谢雨欣提供补丁)

2022/11/16
  * 增加函数wcscasecmp(感谢雨欣提供补丁)

2022/11/11
  * 完善fscript解析。
  * 完善json对\\的处理。
  * 删除导出idle的时候多 余的打印信息(感谢智明提供补丁)

2022/11/09
  * 在Windows下将wcscasecmp定义为wcsicmp(感谢雨欣提供补丁)

2022/11/07
  * 增加函数value\_min/value\_max。

2022/11/03
  * 导出conf\_node\_count\_children函数(感谢智明提供补丁)

2022/11/01
  * 修复windows下如果出现串口异常 的时候会导致检查线程没有睡眠的问题(感谢智明提供补丁)

2022/10/31
  * 修复 awtk 导出 def 时缺少部分的 get\_widget\_vtable 函数的问题(感谢智明提供补丁)

2022/10/27
  * 增加函数value\_mod/value_\_expt

2022/10/26
  * 增加函数value\_abs/value\_add/value\_sub/value\_mul/value\_div

2022/10/20
  * 增加tk\_object\_to\_json函数。

2022/10/15
  * 增加str\_from\_xxx函数。

2022/10/12
  * 完善idl_gen支持导出vtable函数(感谢智明提供补丁)

2022/10/11
  * 完善tk\_istream\_read\_line超时处理(感谢忠吉提供补丁)

2022/10/09
  * 增加tk_strnlen和wcsncmp函数让stm32项目编译通过(感谢智明提供补丁)

2022/09/27
  * 把windows下加载res资 源的逻辑移动在awtk_config.py文件中(感谢智明提供补丁) 

2022/09/26
  * 增加value\_lshift/value\_rshift/value\_lshift\_r/value\_rshift\_r等函数。
  * 增加value\_set\_bit/value\_get\_bit/value\_toggle\_bit等函数。
  * 增加value\_bit\_not/value\_bit\_and/value\_biti\_or等函数。

2022/09/23
  * 完善rbuffer\_read\_string接口(感谢雨欣提供补丁)

2022/09/22
  * 完善fscript单元测试(感谢雨欣提供补丁)
  * 增加判断目录是否为空的接口(感谢兆坤提供补丁)
  * 完善types_def.h(感谢文静提供补丁)

2022/09/21
  * 完善编译脚本对mingw的支持(感谢俊杰提供补丁) 
  * 消除fscript_array中不必要的警告(感谢雨欣提供补丁)
  * 完善fscript_str的相关接口，返回值采用dup形式 (感谢雨欣提供补丁)

2022/09/20
  * 修改 tkc 支持 windows 下加载 res 资源(感谢智明提供补丁)
  * 使用scons清理时把bin文件夹中的sharedlib删除(感谢兆坤提供补丁)
  * fscript_array新增虚函数表(感谢雨欣提供补丁)。
  * 修复str_replace修改容量导致fscript中replace接口返回值有误的问题(感谢雨欣提供补丁)。
  * 完善fscript 中 fformat(感谢雨欣提供补丁)。

2022/09/16
  * path_dirname 处理斜杠和反斜杠的混用的路径时候不正常的问题(感谢智明提供补丁)
  * 修复awtk只有dll没有lib文件复制失败的问题(感谢兆坤提供补丁)

2022/09/14
  * 支持在添加idle和添加timer是指定ID(感谢智明提供补丁)

2022/09/09
  * 完善linux串口打开失败时的处理(感谢忠吉提供补丁)

2022/09/07
  * 解决Windows平台网络接口的一个bug：网卡名不正确时，接口应返回NULL(感谢忠吉提供补丁)

2022/09/05
  * tkc修复tk_memcpy拷贝异常的问题 (感谢智明提供补丁)

2022/09/01
  * 修复path\_normalize函数多次返回上一级的时候会数据出现不正常的问题(感谢智明提供补丁)

2022/08/31
  * wstr\_set\_utf8\_with\_len接口设置失败时打印警告并返回参数异常(感谢雨欣提供补丁)
  * 修复fscript中一处object\_t没有加tk前缀的问题(感谢创建提供补丁)。

2022/08/30
  * wstr\_set\_utf8接口设置失败时打印警告并返回参数异常(感谢雨欣提供补丁)

2022/08/28
  * 修复fscript调用type\_array\_get时，传参有误导致程序崩溃的问题(感谢雨欣提供补丁)

2022/08/23
  * 完善 fscript\_locals\_get
  * 完善 fscripts 示例。

2022/08/22
  * 完善 data\_reader\_factory\_create\_reader处理Windows的盘符(感谢ufbycd提供补丁)。

2022/08/17
  * 完善conf\_xml，支持通过@text访问文本内容。
  * 修复编译警告(感谢智明提供补丁)
  * 完善编译脚本(感谢智明提供补丁)
  * 交叉编译的时候，可以全局配置交叉编译工具的链接库路径(感谢智明提供补丁)

2022/08/15
  * 完善conf\_xml。
  * 完善conf\_obj\_create\_sub\_object

2022/08/13
  * 修复编译警告(感谢雨欣提供补丁)

2022/08/03
  * fix memory leak
  * improve fscript get/unset

2022/08/02
  * 增加fscript_clean。

2022/08/01
  * 增加fscript\_find\_func。

2022/07/29
  * 增加函数object\_array\_remove\_value
  * 更新fs\_aworks.c(感谢文浩提供补丁)

2022/07/22
  * debugger增加restart函数(感谢智明提供补丁)
  * 完善debugger，调试结束后从列表中移除调试器。

2022/07/21
  * 修复低版本gcc编译fscript\_test报错的问题 (感谢雨欣提供补丁）

2022/07/19 
  * fscript支持实例的hooks(感谢智明提供补丁)
  * 修复 darray\_remove\_range 编译警告(感谢智明提供补丁)

2022/07/17
  * 增加 darray\_remove\_range (感谢智明提供补丁)

2022/07/13
  * tk\_atoi/tk\_atol/tk\_atoul等函数视'#'开头的字符串为十六进制。

2022/07/08
  * 完善serial helper
  * 增加函数 tk\_iostream\_serial\_wait\_for\_data


2022/07/07
  * 将tk\_is\_ui\_thread移动到utils.c中。
  * 完善str\_set\_with\_len。
  * 修复打印ubjson的时候出现二进制断言的问题(感谢智明提供补丁)

2022/07/06
  * fscript支持ret\_t中的常量。
  * 增加函数path\_prepend\_app\_root
  * 增加函数tk\_object\_set\_prop\_pointer\_ex
  * 增加函数value\_set\_pointer\_ex

2022/06/30
  * 增加函数 fscript\_get\_code\_id

2022/06/29
  * 完善 Windows 的串口(感谢智明提供补丁)

2022/06/27
  * 增加 fs\_copy\_dir\_ex

2022/06/25
  * 完善 platform\_prepare

2022/06/20
  * 增加 log\_if\_fail。

2022/06/17
  * 完善str\_unescape

2022/06/12
  * conf doc 支持向数组中追加数据。 

2022/06/07
  * 完善fscript调试器(感谢兆坤提供补丁)。

2022/06/04
  * 增加函数 conf\_doc\_set\_node\_prop 

2022/06/03
  * 增加函数 conf\_doc\_node\_dup

2022/05/31
  * 增加函数str\_encode\_xml\_entity/str\_encode\_xml\_entity\_with\_len。
  * 增加conf\_xml用于读写xml文件。  

2022/05/27
  * 增加函数tk\_replace\_char
  * typed\_array支持bool类型(感谢兆坤提供补丁)。
  * 去除debugger警告log (感谢兆坤提供补丁)。
  * 增加debugger\_client分发其他消息处理(感谢兆坤提供补丁)。

2022/05/26
  * 增加函数tokenizer\_next\_str，用于解析带引号的字符串。
  * 完善func\_call\_parser，支持带引号的参数。

2022/05/25
  * 完善编辑脚本（感谢朝泽提供补丁）
    * scons -c时不执行app_helper的prepare()
    * 修复项目编译时由于awtk的idl.json不存在导致idl_gen失败的问题

2022/05/20
  * 增加EMITTER\_ENABLE/EMITTER\_DISABLE

2022/05/19
  * 完善func\_json\_save\_ex（感谢国文提供补丁）。
  * 增加函数tk_memdup
  * 增加fscript函数binary
  * 完善func\_str

2022/05/18
  * 完善Windows下utf8转换。按照utf16的规则，对大于0x10000的字符值进行正确的编码 (感谢俊圣提供补丁)

2022/05/16
  * 完善 utf16\_to\_utf8  拷贝不下, 返回NULL,  避免崩溃或越界访问(感谢俊圣提供补丁)
  * 完善 tk\_utf8\_dup\_utf16 (感谢俊圣提供补丁)

2022/05/13
  * 修复 fscript中json\_load参数个数大于1时的问题（感谢国文提供补丁）。

2022/05/11
  * 完善 conf\_obj\_save

2022/05/02
  * 增加 tk\_object\_copy\_props。

2022/04/26
  * 完善 stream serial 的文档（感谢智明提供补丁）。

2022/04/24
  * 增加 weak 机制以及在 g2d 和 raw 中增加 weak （感谢智明提供补丁）。

2022/04/23
  * 完善 socket\_resolve 防止溢出（感谢叶冬提供补丁）
  * 完善 tk\_socket\_close 支持 AWorks（感谢叶冬提供补丁）
  * 增加 async\_call\_init\_ex2（感谢叶冬提供补丁）

2022/04/22
  * 增加 tk\_eval\_ratio\_or\_px
  * 增加 TK\_STR\_IS\_EMPTY
  * 增加 TK\_STR\_IS\_NOT\_EMPTY
  * 完善 idl_gen（感谢王程提供补丁）。

2022/04/19
  * 完善 tk\_istream\_read\_line

2022/04/18
  * 增加函数 path\_app\_root\_ex
  * awtk 修复缺少连接 ws2_32 类库的问题 （感谢智明提供补丁）。
  
2022/04/16
  * 增加函数 tk\_socket\_bind\_ex
  * 增加函数 tk\_iostream\_udp\_create\_client\_ex

2022/04/14
  * 让 windows 的串口也支持 select 的形式（感谢智明提供补丁）。

2022/04/09
  * fscript 增加 define\_param 用于辅助反向生成 AWBlock。
  * 完善 fscript 语法检查

2022/04/07
  * 增加函数 url\_to\_string。

2022/04/01
  * 完善 URL 解析并增加一些新的 schema。
  * 完善 ubjson writer

2022/03/31
  * 增加带统计功能的 stream （感谢林福提供）
  * 增加移除和替换 qaction 的 action\_darray\_thread（感谢林福提供）

2022/03/30
  * fscript 支持延迟解析函数。
  * 完善 fscript 局部变量的访问。
  * fscript 支持通过 require 函数加载模块。

2022/03/29
  * 优化 fscript 局部变量的访问。
  * 更新 aworkslp 移植代码（感谢文静提供）
  * 删除 mingw 的 UNICODE 宏和 msvc 统一宏的版本（感谢智明提供补丁）。

2022/03/28
  * 完善 fscript。

2022/03/23
  * 改用 mingw 默认的宏来定义 MINGW 宏（感谢智明提供补丁）。
  * 完善 ubjson 对字符的处理（感谢攀峰提供补丁）

2022/03/22
  * 完善 mingw 编译（感谢智明提供补丁）。

2022/03/21
  * 完善 mingw 编译（感谢智明提供补丁）。

2022/03/20
  * 完善 ubjson\_writer\_write\_object。
  * 完善 fscript 函数 len

2022/03/19
  * 完善 fscript 函数 bit\_not
  * 完善 fargs\_get\_data\_and\_size
  * 完善 object\_date\_time 支持 foreach。

2022/03/18
  * 完善 tk_\cond\_var\_wait（感谢兆坤提供补丁）。

2022/03/16
  * 增加通配符匹配函数 tk\_wild\_card\_match。

2022/03/10
  * 完善 fscript debugger

2022/03/04
  * 修改 csv 中的内存泄漏（感谢林福提供补丁）。

2022/03/02
  * AWorks 的 serial\_helper 增加设置超时的接口（感谢林福提供补丁）。

2022/02/24
  * 增加函数 str\_format/str\_append\_format。

2022/02/23
  * 修复导出 IDL 文件的时候因为查找不到依赖的类导致出现警告（感谢智明提供补丁）。
  * 集成了 IoT.js，所以同步更新 app\_helper\_base.py（感谢朝泽提供补丁）
  * 修复编译警告（感谢智明提供补丁）。

2022/02/22
  * 完善 fscript 调试器。

2022/02/21
  * 修复 fscript global 对象初始化的问题。
  * 修复 value\_int64/value\_uint64 对大数的处理。

2022/02/18
  * 增加函数 fscript\_set\_global\_object。

2022/02/15
  * conf 对象支持 foreach\_prop。
  * conf 支持创建子对象。

2022/02/10
  * 完善 API 注释（感谢朝泽提供补丁）
  * 完善 str\_append\_json\_str。

2022/02/04
  * 增加函数 path\_basename\_ex。

2022/02/02
  * 完善 fscript 语法错误检查。

2022/01/29
  * 完善 fscript debugger

2022/01/24
  * 优化 tk\_strncpy（感谢智明提供补丁）
  * 完善 fscript debugger

2022/01/23
  * 完善 fscript debugger, 支持 attach/launch 两种启动方式。

2022/01/22
  * 完善 fscript debugger

2022/01/21
  * 完善 fscript debugger 并增加 fdb 命令行调试工具。
  * 完善 aworks 平台的 serial\_helper.c（感谢文静提供）

2022/01/20
  * 增加 fscript 的构造函数和析构函数（感谢智明提供补丁）

2022/01/19
  * 完善 fscript 调试器（支持 step in/step out/step over/reload code)。

2022/01/16
  * 完善 fscript 支持调试器。

2022/01/13
  * 优化 tk\_object\_get\_child\_object（感谢雨欣提供补丁）

2022/01/11
  * 修复 WITHOUT\_FSCRIPT 没有生效的问题。
  * 完善 fscript 表达式。

2022/01/10
  * 完善 fscript if/elseif/else 语句
  
2022/01/08
  * fscript 的 log 函数改名为 logf，避免与 flow 中的 log 重名。
  * fscript 增加 date\_time\_get\_prop/date\_time\_set\_prop 等函数。 

2022/01/07
  * fscript 增加 str\_is\_empty/str\_len/array\_is\_empty/array\_size 等函数。
  * object\_typed\_array 支持通过 TK\_OBJECT\_PROP\_SIZE 获取元素个数。
  * object\_typed\_array 支持通过 set\_prop/get\_prop 访问元素。
  * fscript 支持 for/for\_in 语句。

2022/01/06
  * fscript 增加 random0to1。
  * fscript 支持 until/repeat_times/repeat 语句。
  * fscript 中增加 is\_int/is\_odd/is\_even 等函数

2022/01/05
  * fscript 公开函数 value\_id/value\_func。
  * fscript 增加 log10/pow10 等函数。
  * 修复注释错误（感谢朝泽提供补丁）
  * fscript 中三角函数增加以度数为单位的相应函数。

2022/01/04
  * fscript 支持保存函数名，方便调试。

2022/01/02
  * 修复 emitter 被覆盖的问题（感谢智明提供补丁）
  * 修复 fscript 中 while 语句对 return 的处理。

2022/01/01
  * 优化 fscript 函数调用。
  * 公开结构 fscript\_func\_call\_t。
  * fscript 支持设置错误处理函数。
  * fscript 增加函数 sleep\_ms。

2021/12/31
  * fscript 允许重载 print 函数。
  * 修复 emitter 的事件中删除事件同时递归触发事件导致 emitter 不正常的问题（感谢 zxc3489 和智明提供补丁）
  * 修复使用 thread\_with\_pthread 时，线程运行完毕后调 用 join 函数出现内存泄漏的问题 （感谢雨欣提供补丁）
  * fscript 去掉 a,b,c,d 快速变量，a,b,c,d 和普通变量一样处理。

2021/12/29
  * 修复 fscript 中 unset 的问题。
  * fscript 增加 f64/double 数据类型转换函数。

2021/12/28
  * 完善 serial\_helper.c（感谢朝泽提供补丁）

2021/12/26
  * 增加函数 array\_reverse
  * 增加 fscript 函数 array\_reverse

2021/12/25
  * 增加函数 str\_reverse/str\_count。
  * 增加 fscript 函数 text\_reverse/text\_count。

2021/12/24
  * 增加状态机（感谢林福提供补丁）
  * 修复单元测试：兼容非 utf8 编码的编译器（VS2015）（感谢雨欣提供补丁）
  * 完善部分 cast 函数的文档。

2021/12/23
  * 增加 aworkslp 平台支持（感谢林福提供补丁）
  * fscript 增加 code\_id，方便以后支持调试。
  * 编译嵌入式 linux 不使用 WITH\_SDL 宏，以及中文编码问题（感谢陈谭提供补丁）
  * 修正 Win32 串口操作速度被限制的问题（感谢陈谭提供补丁）

2021/12/22
  * 复原被覆盖的 tools/idl\_gen/README.md（感谢雨欣提供补丁）
  * 增加 tkc/sha256.h

2021/12/20
  * fscript 支持全局对象。

2021/12/11
  * 增加 tk\_str\_is\_in\_array。
  * 增加 tk\_utf8\_trim\_invalid\_char。

2021/12/09
  * 完善 json 解析对整数的处理。

2021/12/02
  * TKC 从 AWTK 中独立出来。
  * 重构编译脚本，增加 awtk\_config\_common.py。
  * 一些可能重名函数增加 tk_前缀。

2021/12/06
  * 支持 linux 系统交叉编译（感谢智明提供补丁）。
