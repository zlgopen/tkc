## 目录介绍

* **tkc** toolkit c library。
* **misc** C++支持等。			
* **xml** XML解析器。
* **platforms** 平台相关的函数。
* **ubjson** Universal Binary JSON实现。
* **streams** 各种流的实现(如果不用可以不移植)。
* **compressors** 压缩算法的实现(如果不用可以不移植)。
* **conf_io** json/jni/ubjson 等配置文件读写。
* **csv** csv 文件读写。

## 使用方法

如果只使用 tkc 核心库，包含 tkc.h 即可。

```c
#include "tkc.h"
```

如果使用 xml/ubjson/conf_io 等扩展库，则需要包含对应的头文件。如：


```c
#include "conf_io/conf_json.h"
```



