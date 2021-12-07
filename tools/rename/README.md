# 说明

为了避免函数名冲突，给 object 和其它部分函数增加了 tk_前缀，本目录的工具用于更新使用 tkc/awtk 的应用程序。

> 如果没有定义 USE_TK_PREFIX, tkc 会定义一组宏兼容之前的函数名，应用程序无需修改。

## 使用方法

* 将 tools/rename 拷贝到自己项目的同名目录。

比如项目叫 hello，那么拷贝后 rename 目录的位置为：

```
hello/tools/rename
```

* 修改 rename.sh 脚本，加入需要替换的子目录和文件类型。

* 在项目根目录下运行脚本

```
./tools/rename/rename.sh
```

* 用 git diff 或其它工具查看修改是否有误。

* 编译并测试。
