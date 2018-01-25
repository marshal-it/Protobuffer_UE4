echo build current dir's proto file
echo -------------------开始编译----------------

::编译当前目录下的所有的proto文件
::protoc.exe -I=e:\test --cpp_out=e:\test e:\test\test.proto

set dir=%cd%

:: -I
set inpath=%dir%
:: --cpp_out
set out_cpp=%dir%
:: e:\test.proto
set protofile=%dir%\test.proto

for /R %%s in (*.proto) do (
	if exist %%s protoc.exe -I=%dir% --cpp_out=%out_cpp% %%s
)

::protoc.exe -I=%dir% --cpp_out=%out_cpp% %protofile%

echo 当前文件夹：%inpath%
echo 当前文件输出路径：%out_cpp%
echo 当前需要编译的文件：%protofile%

echo end build
echo ----------------------编译完成-------------------

pause