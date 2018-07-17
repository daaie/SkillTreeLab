// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ConstructorHelpers.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Animation/AnimInstance.h"
#include "MySceneComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeLocation(FVector(0, 30, 70));
	SpringArm->TargetArmLength = 150.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Male(
		TEXT("SkeletalMesh'/Game/Male_Grunt/Mesh/male_grunt.male_grunt'"));
	if (SK_Male.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Male.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	bUseControllerRotationPitch = false;
	SpringArm->bUsePawnControlRotation = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	FStringClassReference Anim_Male(TEXT("AnimBlueprint'/Game/TPS/Blueprints/BP_AnimCharacter.BP_AnimCharacter_C'"));
	if (UClass* Anim_Male_Class = Anim_Male.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Male_Class);
	}

	Weapon = CreateDefaultSubobject<UMySceneComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_M4A1(TEXT("StaticMesh'/Game/Weapons/M4A1/SM_M4A1.SM_M4A1'"));
	if (SK_M4A1.Succeeded())
	{
		Weapon->SetStaticMesh(SK_M4A1.Object);
	}

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprint && GetCharacterMovement()->Velocity.SizeSquared() == 0)
	{
		bIsSprint = false;
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Pressed, this, &AMyCharacter::TryIronsight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::UnSprint);

	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Pressed, this, &AMyCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Released, this, &AMyCharacter::UndoLookAround);
}

void AMyCharacter::TryIronsight()
{
	bIsIronsight = bIsIronsight ? false : true;

	if (!bIsIronsight)
	{
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AMyCharacter::TryCrouch()
{
	if (CanCrouch() && !bIsSprint)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsSprint)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}
void AMyCharacter::Turn(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}
void AMyCharacter::LookUp(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerPitchInput(Value);

	}
}

FRotator AMyCharacter::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);

	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void AMyCharacter::Sprint()
{
	if (!bIsCrouched && !bIsIronsight)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::LookAround()
{
	if (!bIsIronsight)
	{
		bUseControllerRotationYaw = false;
	}
}

void AMyCharacter::UndoLookAround()
{
	bUseControllerRotationYaw = true;
}

void AMyCharacter::UnSprint()
{
	if (bIsSprint)
	{
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}
