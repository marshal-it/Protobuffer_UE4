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
#include "test.pb.h"

using namespace tutorial;
using namespace std;




UProtoBufferBPLibrary::UProtoBufferBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UProtoBufferBPLibrary::ProtoBufferSampleFunction(float Param)
{
	//check
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	tutorial::Person person;

	//write data in person.pb file
	person.set_id(123);
	person.set_name("Bob");
	person.set_email("bob@example.com");

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

#ifdef _MSC_VER
#include "HideWindowsPlatformTypes.h" 
#endif