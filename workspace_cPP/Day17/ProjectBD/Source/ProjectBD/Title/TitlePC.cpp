// Fill out your copyright notice in the Description page of Project Settings.

#include "TitlePC.h"
#include "Title/TitleWidgetBase.h"
#include "Http.h"
#include "Json.h"
#include "Misc/Base64.h"


void ATitlePC::BeginPlay()
{
	HttpConnection = &FHttpModule::Get();

	FStringClassReference TitleWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Title/TitleWidget.TitleWidget_C'"));
	if (UClass* MyWidgetClass = TitleWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		//#include "TitleWidget"
		//TitleWidget = Cast<UTitleWidgetBase>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		TitleWidget = Cast<UTitleWidgetBase>(NewObject<UUserWidget>(this, MyWidgetClass));
		//new

		TitleWidget->AddToViewport();
	}

	FStringClassReference LoadingWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UI/Loading/LoadingWidget.LoadingWidget_C'"));
	if (UClass* MyWidgetClass = LoadingWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		LoadingWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(this, MyWidgetClass));
		LoadingWidget->AddToViewport();
		LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	LoginHTTPGet(TEXT("Junios"), TEXT("1234"));
}

void ATitlePC::LoginHTTPGet(const FString & ID, const FString & Password)
{
	TSharedPtr<IHttpRequest> Request = HttpConnection->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ATitlePC::OnRequestComplete);

	FString EncodeID = FGenericPlatformHttp::UrlEncode(ID);

	FString url = FString(TEXT("http://home.junios.net:8000/ue4/login.php"));
	url = url + FString::Printf(TEXT("?id=%s&password=%s"), *EncodeID, *Password);
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain"));
	Request->ProcessRequest();
}

void ATitlePC::LoginHTTPPost(const FString & ID, const FString & Password)
{
	TSharedPtr<IHttpRequest> Request = HttpConnection->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ATitlePC::OnRequestComplete);

	FString EncodeID = FGenericPlatformHttp::UrlEncode(ID);

	//FBase64::Encode(binary, Text);

	//FString url = FString(TEXT("http://home.junios.net:8000/ue4/login.php"));
	FString url = FString(TEXT("http://127.0.0.1:3000"));
	FString Parameter = FString::Printf(TEXT("id=%s&password=%s&picture=encodeValue"), *EncodeID, *Password);

	//FBase64 class이용 하시면 바이너리 데이터 전송 가능함
	Request->SetURL(url);
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(Parameter);
	Request->ProcessRequest();
}

void ATitlePC::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
	if (bSuccess)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString ID = JsonObject->GetStringField(TEXT("id"));
			int Password = (int)JsonObject->GetNumberField(TEXT("password"));

			GLog->Log(FString::Printf(TEXT("Parsing %s %d"), *ID, Password));
		}
	}
}
