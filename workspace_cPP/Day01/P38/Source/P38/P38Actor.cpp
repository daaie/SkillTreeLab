// Fill out your copyright notice in the Description page of Project Settings.

#include "P38Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// P38Actor.h보다 밑에 선언해야함.

// Sets default values.
AP38Actor::AP38Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

	// LogClass, LogTemp 로그 앞머리같은거.
	// Info, Warning, ERROR.
	// 언리얼은 utf8라서 무조건 TEXT로 묶어줘야함.

	UE_LOG(LogClass, Warning, TEXT("HELLO WORLD"));
	UE_LOG(LogClass, Warning, TEXT("한글은 워닝남 파일에 고급옵션에 파일 인코딩 utf8로바꿔라ㅋㅋㅋ"));


	//컴포넌트 이름 같으면 꺼짐..
	//Create를 하면 new할떄랑 달리 엔진이 관리해줌, 엔진이 안쓰는 클래스만 new.

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(
					TEXT("StaticMesh'/Game/Data/Meshes/SM_P38_Body.SM_P38_Body'"));
	if(SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(
		TEXT("StaticMesh'/Game/Data/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Left->SetRelativeLocation(FVector(37, -20, 0.8f));
	Right->SetRelativeLocation(FVector(37, 20, 0.8f));

	// 엔진이 관리르안하거나 Struct인 경우는 F .
	// 그럼 헤더툴이 안건듬.
}

// Called when the game starts or when spawned.
void AP38Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame.
void AP38Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotatePropeller(Left);
	RotatePropeller(Right);
}


void AP38Actor::RotatePropeller(UStaticMeshComponent* Mesh)
{
	float DeltaSecond = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	Mesh->AddRelativeRotation(FRotator(0, 0, 360 * DeltaSecond));
	Mesh->AddRelativeRotation(FRotator(0, 0, 360 * DeltaSecond));
}
