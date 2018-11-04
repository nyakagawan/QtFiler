set RootDir=%~dp0..
cd %RootDir%
set ExePath=%RootDir%\x64\Release\Filer.exe
set ZipSrc=%RootDir%\x64\Release\*
set ZipPath=%RootDir%\Tools\Release.zip

C:\Qt\5.11.1\msvc2017_64\bin\windeployqt.exe --release %ExePath%

powershell -ExecutionPolicy RemoteSigned -File Tools\Compress.ps1 %ZipSrc% %ZipPath%

@pause