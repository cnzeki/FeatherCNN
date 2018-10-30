<img width="420"  src="https://github.com/Tencent/FeatherCNN/wiki/Images/logo.png"/>

[![license](http://img.shields.io/badge/license-BSD3-blue.svg?style=flat)](https://github.com/Tencent/FeatherCNN/blob/master/LICENSE)
[![Release Version](https://img.shields.io/badge/release-0.1.0-red.svg)](https://github.com/Tencent/FeatherCNN/releases)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/Tencent/FeatherCNN/pulls)

## Introduction

[FeatherCNN](https://github.com/Tencent/FeatherCNN), developed by Tencent TEG AI Platform, is a high-performance lightweight CNN inference library. FeatherCNN is currently targeting at ARM CPUs, and is capable to extend to other devices in the future.

More detail can be found in the original project  [FeatherCNN](https://github.com/Tencent/FeatherCNN). This project port the original codes to the Windows platform, makes it much easier to use.

## Build native so for Android
Install [Android NDK](https://developer.android.google.cn/ndk/downloads/)  and run

```
build_scripts\build_ndk.bat
```

If everything goes well, headers and so file will be in a new  **feather**  directory.



## Build model conversion tool

### Depends
- Visual Studio 2015
- CMake (>3.2)

###  Environment Setup

Open the appropriate *Command Prompt* from the *Start* menu.

For example *VS2015 x64 Native Tools Command Prompt*:

```
C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC>
```

Change to your working directory:

```
> cd /to/project/dir
```

If *cmake* command is not available from *Command Prompt*, add it to system *PATH* variable:

```
> set PATH=%PATH%;C:\Program Files (x86)\CMake\bin
```

Good. Now you are ready to continue.

###  Build tool

```
cd tools
build.bat
```

feather_convert_caffe.exe will be generated in the **tools** directory.