setlocal
set RootDir=%~dp0..
set ExePath=%RootDir%\x64\Release\Filer.exe
set ZipSrc=%RootDir%\x64\Release\*
set ZipPath=%RootDir%\Tools\Filer.zip

call %~dp0ReleaseBuildVS2019.bat

C:\Qt\5.14.1\msvc2017_64\bin\windeployqt.exe --release %ExePath%

powershell -ExecutionPolicy RemoteSigned -File %RootDir%\Tools\Compress.ps1 %ZipSrc% %ZipPath%

endlocal

@pause