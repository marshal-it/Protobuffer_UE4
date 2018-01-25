#include "ProtoBufferPrivatePCH.h"
#include "ProtobufFactory.h"

UProtobufFactory::UProtobufFactory()
{
	InitProto();
}

UProtobufFactory::~UProtobufFactory()
{
}

void UProtobufFactory::LoadProtoAttribute(FString ProtoName, FString PackageName, FString AttName, MsgType msgType, ProtoType AttType, FString& OutValue)
{
	std::string mProtoName(TCHAR_TO_UTF8(*ProtoName));
	std::string mPackageName(TCHAR_TO_UTF8(*PackageName));
	std::string mAttName(TCHAR_TO_UTF8(*AttName));

	google::protobuf::compiler::DiskSourceTree SourceTree;

	SourceTree.MapPath("", ProtoDirPath);
	google::protobuf::compiler::Importer mImporter(&SourceTree, NULL);

	mImporter.Import(mProtoName);

	auto Descriptor = mImporter.pool()->FindMessageTypeByName(mPackageName);
	google::protobuf::DynamicMessageFactory ProtoFactory;
	auto Proto = ProtoFactory.GetPrototype(Descriptor);
	auto Message = Proto->New();
	auto Reflection = Message->GetReflection();

	auto Filed = Descriptor->FindFieldByName(mAttName);
	auto filed1 = Descriptor->FindFieldByName("name");
	auto filed2 = Descriptor->FindFieldByName("email");

	switch (AttType)
	{
	case ProtoType::P_INT32:
	{

		Reflection->SetInt32(Message, Filed, 520);
		Reflection->SetString(Message, filed1, "zhongzhihua");
		Reflection->SetString(Message, filed2, "7317969911@qq.com");
		int size = Message->ByteSize();
		char* data = (char*)malloc(size);
		Message->SerializeToArray(data, size);
		MsgQueue.Add(msgType, data);
		delete Message;
		Message = Proto->New();
		Message->ParseFromArray(MsgQueue[msgType], size);

		//google::protobuf::util stuctjson;

	//	google::protobuf::MessageOptions.Mes

		break;
	}
	case ProtoType::P_INT64:
		break;
	case ProtoType::P_UINT32:
		break;
	case ProtoType::P_UINT64:
		break;
	case ProtoType::P_FLOAT:
		break;
	case ProtoType::P_DOUBLE:
		break;
	case ProtoType::P_BOOL:
		break;
	case ProtoType::P_STRING:
		break;
	case ProtoType::P_ENUM:
		break;
	default:
		break;
	}

	std::string jsonstr = "";
	google::protobuf::util::JsonOptions options;
	bool ok = google::protobuf::util::MessageToJsonString(*Message, &jsonstr, options).ok();

	int32 Out = Reflection->GetInt32(*Message, Filed);
	OutValue = FString::FromInt(Out);
	delete Message;
}

void UProtobufFactory::SetMessage(UJsonFieldData* JsonField, FString ProtoName, FString PackageName)
{
	FString JsonContent;
	JsonField->JsonDataToString(JsonContent);

	std::string mProtoName(TCHAR_TO_UTF8(*ProtoName));   //proto文件名
	std::string mPackageName(TCHAR_TO_UTF8(*PackageName)); //package.message
	std::string mJsonContent(TCHAR_TO_UTF8(*JsonContent));

	google::protobuf::compiler::DiskSourceTree SourceTree;

	SourceTree.MapPath("", ProtoDirPath);
	google::protobuf::compiler::Importer mImporter(&SourceTree, NULL);

	mImporter.Import(mProtoName);

	auto Descriptor = mImporter.pool()->FindMessageTypeByName(mPackageName);
	google::protobuf::DynamicMessageFactory ProtoFactory;
	auto Proto = ProtoFactory.GetPrototype(Descriptor);

	google::protobuf::Message* Message = Proto->New();

	bool ok = google::protobuf::util::JsonStringToMessage(mJsonContent, Message).ok();

	auto Reflection = Message->GetReflection();
	auto filed = Descriptor->FindFieldByName("id");
	auto filed1 = Descriptor->FindFieldByName("name");
	auto filed2 = Descriptor->FindFieldByName("email");

	std::string name = Reflection->GetString(*Message, filed1);
	int32 id = Reflection->GetInt32(*Message, filed);
	
	UE_LOG(LogJson, Error, TEXT(" Reflection->GetInt32:\n'%s'"), *FString::FromInt(id));

	//	JsonField->JsonDataToString(JsonField)
	//JsonField->WriteObject(JsonWriter, "", new FJsonValueObject(JsonField->Data))
}

//void UProtobufFactory::Json2Protobuf(UJsonFieldData* JsonField, google::protobuf::Message PbMsg)
//{
//	JsonField->get	
//}

void UProtobufFactory::InitProto()
{
	ProtoDirPath = "E:/UE4WorkSpace/NDXIntegrateV2/Plugins/ProtoBuffer/Source/ProtoBuffer/Proto/";
}


