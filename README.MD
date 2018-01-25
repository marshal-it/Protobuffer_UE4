google protobuf in ue4.16

this is a ue4 plugin , you only copy this protobuffer plugin in your project's plugins directory.

and a test.proto file have used in plugin , you can see ProtoBufferLibrary.cpp in plugin.

protobuf version is 3.5.1

[tip]: have a "libprotobufd.zip" in "Protobuffer_UE4\ProtoBuffer\ThirdParty\lib\Debug",you must unzip by self

![image](https://github.com/marshal-it/Protobuffer_UE4/blob/master/ProtoBuffer/Resources/1.png)

![image](https://github.com/marshal-it/Protobuffer_UE4/blob/master/ProtoBuffer/Resources/2.png)

![image](https://github.com/marshal-it/Protobuffer_UE4/blob/master/ProtoBuffer/Resources/3.png)

![image](https://github.com/marshal-it/Protobuffer_UE4/blob/master/ProtoBuffer/Resources/4.png)


update !!!

1.can use dynamic decode .proto file , not need contain some .h and .cc files.

2.add json plugin to handle data in bp, it looks  [pic], so you only to use json struct , don't care protobuf how 
to do.

you can look the code eg that how to do it.

![image](https://github.com/marshal-it/Protobuffer_UE4/blob/master/ProtoBuffer/Resources/10008.png)
