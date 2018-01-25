// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ProtoBufferPrivatePCH.h"

#include "ProtoBufferBPLibrary.h"
#include <fstream>
#include <string>

#ifdef _MSC_VER
#include "AllowWindowsPlatformTypes.h"
#endif
#pragma warning(disable:4800)
#pragma warning(disable:4125)
#pragma warning(disable:4668)
#pragma warning(disable:4647)
#pragma warning(disable:4146)

#include <iostream>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>

#include "test.pb.h"

using namespace tutorial;
using namespace std;

UProtoBufferBPLibrary::UProtoBufferBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UProtoBufferBPLibrary::ProtoBufferSampleFunction(float Param, FString Host, int32 Ip)
{
	Person person;
	fstream out("test.pb", ios::out | ios::binary | ios::trunc);
	person.SerializeToOstream(&out);
	out.close();

	//read date from person.pb file
	fstream in("test.pb", ios::in | ios::binary);
	if (!person.ParseFromIstream(&in)) {
		cerr << "Failed to parse person.pb." << endl;
		exit(1);
	}

	int32 id = person.id();
	string name = person.name();
	string email = "";
	if (person.has_email())
	{
		email = person.email();
	}
	UE_LOG(LogTemp, Log, TEXT(".....person id=%s, person name =%s, person email=%s"), *FString::FromInt(id), UTF8_TO_TCHAR(name.c_str()), UTF8_TO_TCHAR(email.c_str()));
	


	return -1;
}


void UProtoBufferBPLibrary::ReProtobuf(FString ProtoName)
{

	std::string proto_str(TCHAR_TO_UTF8(*ProtoName));

	std::string protopath = "E:/UE4WorkSpace/NDXIntegrateV2/Plugins/ProtoBuffer/Source/ProtoBuffer/Proto/";

	google::protobuf::compiler::DiskSourceTree sourceTree;
	sourceTree.MapPath("", protopath);
	google::protobuf::compiler::Importer importer(&sourceTree, NULL);

	importer.Import(proto_str);

	auto descriptor = importer.pool()->FindMessageTypeByName("tutorial.Person");

	google::protobuf::DynamicMessageFactory factory;
	
	auto proto = factory.GetPrototype(descriptor);

	auto message = proto->New();

	auto reflection = message->GetReflection();

	auto filed = descriptor->FindFieldByName("id");

	reflection->SetInt32(message, filed, 10);
	auto filed1 = descriptor->FindFieldByName("name");
	auto filed2 = descriptor->FindFieldByName("email");

	reflection->SetString(message, filed1, "zhongzhihua");
	reflection->SetString(message, filed2, "7317969911@qq.com");

	int size = message->ByteSize();

	char* data = (char*)malloc(size);

	message->SerializeToArray(data, size);

	std::string str = message->DebugString();

	delete message;

	message = proto->New();

	message->ParseFromArray(data, size);

	std::string msg = message->DebugString();
}

void UProtoBufferBPLibrary::SendMsg()
{
	//check
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	tutorial::Person person;

	//write data in person.pb file
	person.set_id(123);
	person.set_name("Bob");
	person.set_email("bob@example.com");

	//int size = person.ByteSize();
	//char* data = (char*)malloc(size); //?
	//person.SerializeToArray(data, size);

	std::string str;
	str = person.SerializeAsString();
	FString strcontent(str.c_str());

	TArray<uint8> binary;
	binary.SetNum(str.length());
	FMemory::Memcpy(binary.GetData(), str.c_str(), str.length());
//	socketer->Send(binary, binary.Num());

	person.Clear();
}

#ifdef _MSC_VER
#include "HideWindowsPlatformTypes.h" 
#endif