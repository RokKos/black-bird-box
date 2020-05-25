@echo off
pushd %~dp0\..\..\
call %~dp0premake5.exe vs2019
popd
PAUSE