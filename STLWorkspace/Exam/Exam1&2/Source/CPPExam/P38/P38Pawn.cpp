// Fill out your copyright notice in the Description page of Project Settings.

#include "P38Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "P38/P38Rocket.h"

// Sets default values
AP38Pawn::AP38Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//Box->SetupAttachment(RootComponent);
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body
	(TEXT("StaticMesh'/Game/Plane/P38Data/Meshes/SM_P38_Body.SM_P38_Body'"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller
	(TEXT("StaticMesh'/Game/Plane/P38Data/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));

	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Left->SetRelativeLocation(FVector(37, -20, 0.8f));
	Right->SetRelativeLocation(FVector(37, 20, 0.8f));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
}

// Called when the game starts or when spawned
void AP38Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP38Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(this->GetActorForwardVector(), 100 * DeltaTime);

	float DeltaSecond = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	Left->AddRelativeRotation(FRotator(0, 0, 360 * DeltaSecond));
	Right->AddRelativeRotation(FRotator(0, 0, 360 * DeltaSecond));

}

// Called to bind functionality to input
void AP38Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AP38Pawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AP38Pawn::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AP38Pawn::Fire);
}

void AP38Pawn::Fire()
{
	GetWorld()->SpawnActor<AActor>(RocketClass, Arrow->GetComponentLocation(), Arrow->GetComponentRotation());
}

void AP38Pawn::MoveForward(float value)
{
	if (value != 0.0f)
	{
		float DeltaTime = 
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(60 * value * DeltaTime, 0, 0));
	}
}

void AP38Pawn::Turn(float value)
{
	if (value != 0.0f)
	{
		float DeltaTime =
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(0, 0, 60 * value * DeltaTime));
	}
}