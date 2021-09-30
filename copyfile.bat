@echo off
set ROOTDIR=G:\work\Develop\SDK\
set Target=%~dp0\

md %Target%\include
md %Target%\lib
md %Target%\lib_x64
md %Target%\bin

REM 拷贝库文件
xcopy %ROOTDIR%\libSAV\lib\Release\libSAV.dll %Target%\lib /h /s /y
xcopy %ROOTDIR%\BVCSP\lib\Release\BVCSP.dll %Target%\lib /h /s /y
xcopy %ROOTDIR%\bvrtc\lib\Release\bvrtc.dll %Target%\lib /h /s /y
xcopy %ROOTDIR%\bvdisplay\lib\Release\bvdisplay.dll %Target%\lib /h /s /y
xcopy %ROOTDIR%\BVCU\lib\Release\libBVCU.dll %Target%\lib /h /s /y
xcopy %ROOTDIR%\BVCU\lib\Release\libBVCU.lib %Target%\lib /h /s /y

xcopy %ROOTDIR%\libSAV\lib\Release\libSAV_x64.dll %Target%\lib_x64 /h /s /y
xcopy %ROOTDIR%\BVCSP\lib\Release\BVCSP_x64.dll %Target%\lib_x64 /h /s /y
xcopy %ROOTDIR%\bvrtc\lib\Release\bvrtc_x64.dll %Target%\lib_x64 /h /s /y
xcopy %ROOTDIR%\bvdisplay\lib\Release\bvdisplay_x64.dll %Target%\lib_x64 /h /s /y
xcopy %ROOTDIR%\BVCU\lib\Release\libBVCU_x64.dll %Target%\lib_x64 /h /s /y
xcopy %ROOTDIR%\BVCU\lib\Release\libBVCU_x64.lib %Target%\lib_x64 /h /s /y

REM 拷贝头文件
xcopy %ROOTDIR%\BVCU\include\*.h %Target%\include /s /h /y
xcopy %ROOTDIR%\libSAV\include\*.h %Target%\include /s /h /y
xcopy %ROOTDIR%\AAA\include\AAABase.h %Target%\include /s /h /y
