echo build current dir's proto file
echo -------------------begin build----------------

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

echo end build
echo ----------------------build finished-------------------

pause