## 项目说明
besovideo smarteye 客户端库BVCU，提供C接口，接口在BVCU.h头文件中。  
功能主要包括： 
- 登录/下线。
- 命令交互。
- 直播音视频、实时语音对讲、历史音视频。
- 文件传输。
- IM即时通讯。
## 编译说明
- windows 32 位库。注意依赖库的应用程序需要也是WIN32。
- ManagerLayer是对BVCU C接口的C#封装，提供源码，需要自己编译，可以根据需求修改源码。
- BVCU SDK是vs2010编译器编码，lib目录中已经携带c/c++运行环境库，您发布版本时需要注意携带。

## 目录说明
```
|-- include 头文件
|-- lib DLL文件和lib文件
|-- doc 使用文档
|-- demo + 示例工程
|        |-- ManagerLayer 对libBVCU的包装，供C#调用
|        |-- WinFormDemo  Visual C# 2010实现的demo，GUI采用WinForm
|        |-- MFCDemo Visual C++ 2010实现的demo，GUI采用MFC
|-- bin 存放demo编译生成的可执行文件
```
## 源码地址

gitee版本会滞后github.

gitee: https://gitee.com/besovideo/bvcusdk.git
github: https://github.com/besovideo/bvcusdk.git

## 论坛支持
http://bbs.besovideo.com:8067/forum.php?mod=forumdisplay&fid=37&page=1