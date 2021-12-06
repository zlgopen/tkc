# TKC = ToolKit C Library

一套跨平台的基础工具库。其中包括链表、数组、字符串 (UTF8 和 widechar)，事件发射器、值、对象、文
件系统、互斥锁和线程、表达式和字符串解析等等。

## 使用方法

### 下载代码

* 用 git 下载

> [Windows 下的 git](https://git-scm.com/download/win)

**下载代码**

```
git clone https://github.com/zlgopen/tkc.git
```

> 国内用户推荐使用：https://hub.fastgit.org/zlgopen/tkc.git

> Windows 下可以用带 GUI 的 git 下载代码。

**更新代码（在 tkc 目录下运行）**

```
git pull
```

### Mac

如果没有安装 nodejs, scons 和 sdl2，请在终端运行下面的命令 （假定已安装 brew)：

```
brew install scons sdl2 node
```

编译运行 （在终端下，进入 tkc 所在的目录，并运行下列命令）：

```
scons
```

### Ubuntu （版本 >=16)

如果没有安装 scons 和依赖的软件包，请在终端运行下面的命令：

```
sudo apt-get install gcc g++ scons libsndio-dev libgtk-3-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libasound2-dev libibus-1.0-dev fcitx-libs-dev git vim clang-format libharfbuzz-dev nodejs libreadline-dev
```

编译运行 （在终端下，进入 tkc 所在的目录，并运行下列命令）：

```
scons
```

### Windows

如果没有安装 nodejs，请到 [nodejs](https://nodejs.org/en/) 下载并安装。

编译运行 （在命令行模式下，进入 tkc 所在的目录，并运行下列命令）：

```
scons
```

### 所有平台

```
cd tools/idl_gen
npm install
```

## 在独立项目中使用

使用方法和之前使用 AWTK 类似，只是需要在 import scripts.app_helper 之前设置环境变量 TKC_ONLY 为 True。如：

```python
import os 

os.environ['TKC_ONLY'] = 'True'
import scripts.app_helper as app 

helper = app.Helper(ARGUMENTS)
helper.set_tkc_only().add_libs(['foo'])
helper.set_dll_def('src/foo.def').call(DefaultEnvironment)

SConsFiles = [ 
    'src/SConscript',
    'demos/SConscript',
    'tests/SConscript',
]

SConscript(SConsFiles)
```

完整项目请参考：[tkc-hello](https://github.com/zlgopen/tkc-hello)

## 交叉编译

* linux 平台交叉编译，请修改 awtk\_config.py 中的 TOOLS\_PREFIX 和相应参数。

> 依赖 tkc 的应用程序无需修改。

## 与 AWTK 的同步

TKC 是从 AWTK 中独立出来的，为了保持兼容性，不影响 AWTK 的用户，TKC 在 AWTK 中的文件位置保持不变，TKC 有更新时，在 AWTK 中运行脚本 sync_tkc.sh 进行文件同步。
