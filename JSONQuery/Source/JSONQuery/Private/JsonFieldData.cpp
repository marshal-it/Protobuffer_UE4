// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#include "JSONQueryPrivatePCH.h"

//////////////////////////////////////////////////////////////////////////
// UJsonFieldData

/**
* Constructor
*/
UJsonFieldData::UJsonFieldData(const class FObjectInitializer& PCIP)
	: Super(PCIP) {
	Reset();
}

/**
* Grabs a page from the internet
*
* @param	WorldContextObject		The current context
* @param	url						The URL to request
*
* @return	A pointer to the newly created post data
*/
UJsonFieldData* UJsonFieldData::GetRequest(UObject* WorldContextObject, const FString &url) {
	// Create new page data for the response
	UJsonFieldData* dataObj = Create(WorldContextObject);

	// Create the HTTP request
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(CreateURL(url));
	HttpRequest->OnProcessRequestComplete().BindUObject(dataObj, &UJsonFieldData::OnReady);
	
	// Execute the request
	HttpRequest->ProcessRequest();

	// Return the page data
	return dataObj;
}

/**
* Create a new instance of the UJsonFieldData class, for use in Blueprint graphs.
*
* @param	WorldContextObject		The current context
*
* @return	A pointer to the newly created post data
*/
UJsonFieldData* UJsonFieldData::Create(UObject* WorldContextObject) {
	// Get the world object from the context
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	// Construct the object and return it
	UJsonFieldData* fieldData = NewObject<UJsonFieldData>();
	fieldData->contextObject = WorldContextObject;
	return fieldData;
}

/**
* Prefixes the input URL with http:// if necessary
*
* @param	inputURL		The input URL
*
* @return	The output URL
*/
FString UJsonFieldData::CreateURL(FString inputURL) {
	if (!inputURL.StartsWith("http")) {
		return "http://" + inputURL;
	}

	return inputURL;
}

/**
* This function will write the supplied key and value to the JsonWriter
*
* @param	writer			The JsonWriter to use
* @param	key				Object key
* @param	value			Object value
*
*/
void UJsonFieldData::WriteObject(TSharedRef<TJsonWriter<TCHAR>> writer, FString key, FJsonValue* value) {
	if (value->Type == EJson::String) {
		// Write simple string entry, don't a key when it isn't set
		if (key.Len() > 0) {
			writer->WriteValue(key, value->AsString());
		} else {
			writer->WriteValue(value->AsString());
		}
	}
	else if (value->Type == EJson::Number)
	{// Write number entry
		if (key.Len() > 0)
		{
			writer->WriteValue(key, value->AsNumber());
		}
		else
		{
			writer->WriteValue(value->AsNumber());
		}
	}
	else if (value->Type == EJson::Boolean)
	{// Write bool entry
		if (key.Len() > 0)
		{
			writer->WriteValue(key, value->AsBool());
		}
		else
		{
			writer->WriteValue(value->AsBool());
		}
	}
	else if (value->Type == EJson::Object) {
		// Write object entry
		if (key.Len() > 0) {
			writer->WriteObjectStart(key); }
		else {
			writer->WriteObjectStart(); }

		// Loop through all the values in the object data
		TSharedPtr<FJsonObject> objectData = value->AsObject();
		for (auto objectValue = objectData->Values.CreateIterator(); objectValue; ++objectValue) {
			// Using recursion to write the key and value to the writer
			WriteObject(writer, objectValue.Key(), objectValue.Value().Get());
		}

		writer->WriteObjectEnd();
	} else if (value->Type == EJson::Array) {
		// Process array entry
		writer->WriteArrayStart(key);
			
		TArray<TSharedPtr<FJsonValue>> objectArray = value->AsArray();
		for (int32 i = 0; i < objectArray.Num(); i++) {
			// Use recursion with an empty key to process all the values in the array
			WriteObject(writer, "", objectArray[i].Get());
		}

		writer->WriteArrayEnd();
	}
}

void UJsonFieldData::JsonDataToString(FString& OutStr)
{
	FString outStr;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
	WriteObject(JsonWriter, "", new FJsonValueObject(Data));
	JsonWriter->Close();

	OutStr = outStr;

	UE_LOG(LogTemp, Warning, TEXT("Post data: %s"), *outStr);
}

/**
* Posts the current request data to the internet
*
* @param	WorldContextObject		The current context
* @param	url						The URL to post to
*
*/
void UJsonFieldData::PostRequest(UObject* WorldContextObject, const FString &url) {
	FString outStr;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
	
	// Start writing the response
	WriteObject(JsonWriter, "", new FJsonValueObject(Data));
	JsonWriter->Close();

	// Log the post data for the user (OPTIONAL)
	UE_LOG(LogTemp, Warning, TEXT("Post data: %s"), *outStr);

	// Create the post request with the generated data
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(CreateURL(url));
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetContentAsString(outStr);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UJsonFieldData::OnReady);

	// Execute the request
	HttpRequest->ProcessRequest();
}

/**
* Adds the supplied string to the post data, under the given key
*
* @param	key						Key
* @param	value					Object value
*
* @return	The object itself
*/
UJsonFieldData* UJsonFieldData::SetString(const FString& key, const FString& value) {
	Data->SetStringField(*key,*value);
	return this;
}

UJsonFieldData* UJsonFieldData::SetNumber(const FString& key, float value)
{
	Data->SetNumberField(*key, value);
	return this;
}

/**
* Adds the supplied object to the post data, under the given key
*
* @param	key						Key
* @param	objectData				Object data
*
* @return	The object itself
*/
UJsonFieldData* UJsonFieldData::SetObject(const FString& key, const UJsonFieldData* objectData) {
	Data->SetObjectField(*key, objectData->Data);
	return this;
}

/**
* Adds the supplied object array to the post data, under the given key
*
* @param	key						Key
* @param	objectData				Array of object data
*
* @return	The object itself
*/
UJsonFieldData* UJsonFieldData::SetObjectArray(const FString& key, const TArray<UJsonFieldData*> objectData) {
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	// Loop through the array and create new shared FJsonValueObject instances for every FJsonObject
	for (int32 i = 0; i < objectData.Num(); i++) {
		dataArray->Add(MakeShareable(new FJsonValueObject(objectData[i]->Data)));
	}

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetBool(const FString& key, bool value)
{
	Data->SetBoolField(*key, value);
	return this;
}

/**
* Adds the supplied string array to the post data, under the given key
*
* @param	key						Key
* @param	objectData				Array of strings
*
* @return	The object itself
*/
UJsonFieldData* UJsonFieldData::SetStringArray(const FString& key, const TArray<FString> stringData) {
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	// Loop through the input array and add new shareable FJsonValueString instances to the data array
	for (int32 i=0; i < stringData.Num(); i++) {
		dataArray->Add(MakeShareable(new FJsonValueString(stringData[i])));
	}

	Data->SetArrayField(*key, *dataArray);
	return this;
}

/**
* Gets the post data object from the post data with the given key
*
* @param	WorldContextObject		Array of strings
* @param	key						Key
*
* @return	The object itself
*/
UJsonFieldData* UJsonFieldData::GetObject(const FString& key) {
	UJsonFieldData* fieldObj = NULL;

	// Try to get the object field from the data
	const TSharedPtr<FJsonObject> *outPtr;
	if (!Data->TryGetObjectField(*key, outPtr)) {
		// Throw an error and return NULL when the key could not be found
		UE_LOG(LogJson, Error, TEXT("Entry '%s' not found in the field data!"), *key);
		return NULL;
	}

	// Create a new field data object and assign the data
	fieldObj = UJsonFieldData::Create(contextObject);
	fieldObj->Data = *outPtr;

	// Return the newly created object
	return fieldObj;
}

/**
* Gets a string array from the post data with the given key
*
* @param	key						Key
*
* @return	The requested array of strings
*/
TArray<FString> UJsonFieldData::GetStringArray(const FString& key) {
	TArray<FString> stringArray;

	// Try to get the array field from the post data
	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr)) {
		// Iterate through the array and use the string value from all the entries
		for (int32 i=0; i < arrayPtr->Num(); i++) {
			stringArray.Add((*arrayPtr)[i]->AsString());
		}
	} else {
		// Throw an error when the entry could not be found in the field data
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key); 
	}

	// Return the array, if unsuccessful the array will be empty
	return stringArray;
}

/**
* Gets an object array from the post data with the given key
*
* @param	key						Key
*
* @return	The requested post data objects
*/
TArray<UJsonFieldData*> UJsonFieldData::GetObjectArray(UObject* WorldContextObject, const FString& key) {
	TArray<UJsonFieldData*> objectArray;

	// Try to fetch and assign the array to the array pointer
	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr)) {
		// Iterate through the input array and create new post data objects for every entry and add them to the objectArray
		for (int32 i = 0; i < arrayPtr->Num(); i++) {
			UJsonFieldData* pageData = Create(WorldContextObject); 
			pageData->Data = (*arrayPtr)[i]->AsObject();
			objectArray.Add(pageData);
		}
	}
	else {
		// Throw an error, since the value with the supplied key could not be found
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
	}

	// Return the array, will be empty if unsuccessful
	return objectArray;
}

/**
* Gets the keys from the supplied object
*
* @param	key						Key
*
* @return	Array of keys
*/
TArray<FString> UJsonFieldData::GetObjectKeys(UObject* WorldContextObject) {
	TArray<FString> stringArray;

	for (auto currJsonValue = Data->Values.CreateConstIterator(); currJsonValue; ++currJsonValue) {
		stringArray.Add((*currJsonValue).Key);
	}

	// Return the array, will be empty if unsuccessful
	return stringArray;
}

/**
* Tries to get a string from the field data by key, returns the string when successful
*
* @param	key			Key
*
* @return	The requested string, empty if failed
*/
FString UJsonFieldData::GetString(const FString& key) const {
	FString outString;

	// If the current post data isn't valid, return an empty string
	if (!Data->TryGetStringField(*key, outString)) {
		UE_LOG(LogJson, Error, TEXT("Entry '%s' not found in the field data!"), *key);
		return "";
	}

	return outString;
}

/**
* Resets the current page data
*
*/
void UJsonFieldData::Reset() {
	// If the post data is valid
	if (Data.IsValid()) {
		// Clear the current post data
		Data.Reset();
	}

	// Create a new JSON object
	Data = MakeShareable(new FJsonObject());
}

/**
* Creates new data from the 
*
* @param	key			Key
*
* @return	The requested string, empty if failed
*/
void UJsonFieldData::FromString(const FString& dataString) {
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(dataString);

	// Deserialize the JSON data
	bool isDeserialized = FJsonSerializer::Deserialize(JsonReader, Data);

	if (!isDeserialized || !Data.IsValid()) {
		UE_LOG(LogJson, Error, TEXT("JSON data is invalid! Input:\n'%s'"), *dataString);
	}

	// Assign the request content
	Content = dataString;
}

/**
* Callback for IHttpRequest::OnProcessRequestComplete()
*
* @param	Request					HTTP request pointer
* @param	Response				Response pointer
* @param	bWasSuccessful			Whether the request was successful or not
*
*/
void UJsonFieldData::OnReady(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful) {
		UE_LOG(LogJson, Error, TEXT("Response was invalid! Please check the URL."));

		// Broadcast the failed event
		OnFailed.Broadcast();
		return;
	}

	// Process the string
	FromString(Response->GetContentAsString());

	// Broadcast the result event
	OnGetResult.Broadcast();
}