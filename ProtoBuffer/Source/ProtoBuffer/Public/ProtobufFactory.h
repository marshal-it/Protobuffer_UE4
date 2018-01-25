/**
@压包和解包
@Author TeenWolf
@Email  731796991@qq.com
@Date   2018-01-17 11:32:51
*/


#pragma once

#include <string>
#include "JsonFieldData.h"
#ifdef _MSC_VER
#include "AllowWindowsPlatformTypes.h"
#endif
#pragma warning(disable:4800)
#pragma warning(disable:4125)
#pragma warning(disable:4668)
#pragma warning(disable:4647)
#pragma warning(disable:4146)

#include <iostream>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include "ProtobufFactory.generated.h"

using namespace std;

UENUM(BlueprintType)
enum class ProtoType : uint8
{
	P_INT32 = 0,
	P_INT64,
	P_UINT32,
	P_UINT64,
	P_FLOAT,
	P_DOUBLE,
	P_BOOL,
	P_STRING,
	P_ENUM
};

UENUM(BlueprintType)
enum class MsgType : uint8
{
	LOGIN = 0,
};


UCLASS(Blueprintable, ClassGroup = (Plugin), meta = (BlueprintSpawnableComponent))
class PROTOBUFFER_API UProtobufFactory : public UActorComponent
{
public:
	GENERATED_BODY()

	UProtobufFactory();
	~UProtobufFactory();

	UFUNCTION(BlueprintCallable, Category = "ProtoBuffer")
		void LoadProtoAttribute(FString ProtoName, FString PackageName, FString AttName, MsgType msgType, ProtoType AttType, FString& OutValue);

	UFUNCTION(BlueprintCallable, Category = "ProtoBuffer")
	void SetMessage(UJsonFieldData* JsonField, FString ProtoName, FString PackageName);

	//void Json2Protobuf(UJsonFieldData* JsonField, google::protobuf::Message PbMsg);

private:

	TMap<MsgType, char*> MsgQueue;

	//{
	//	uint8* msg = MsgQueue[MsgType::LOGIN];

	//	char* msgstr = (char*)msg;

	//}

	//template<ProtoType>
	//void ReflectionByProtoType(ProtoType Type, auto ReflectionRef);

	void InitProto();

	std::string ProtoDirPath;
};

//template<typename T>
//void UProtobufFactory::ReflectionByProtoType(T* Type, auto ReflectionRef)
//{
////	ReflectionRe
//}

#ifdef _MSC_VER
#include "HideWindowsPlatformTypes.h" 
#endif