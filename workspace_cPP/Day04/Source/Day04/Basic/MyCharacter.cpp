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
#include "Player/MySceneComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

#include "Player/MySceneComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Basic/MyCameraShake.h"
#include "BulletDamageType.h"

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

	FStringClassReference Anim_Male(TEXT("AnimBlueprint'/Game/Blueprints/Animation/BP_AnimCharacter.BP_AnimCharacter_C'"));
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

	//WeaponArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponArrow"));
	//WeaponArrow->SetupAttachment(Weapon);
	//WeaponArrow->SetRelativeLocation(FVector(-50, 0, 10));
	//WeaponArrow->SetRelativeRotation(FRotator(0, 180.0f, 0));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;

	NormalSpringArmPosition = SpringArm->GetRelativeTransform().GetLocation();
	ProneSpringArmPosition = FVector(50, SpringArm->GetRelativeTransform().GetLocation().Y, SpringArm->GetRelativeTransform().GetLocation().Z-100);
	CrouchSpringArmPosition = FVector(NormalSpringArmPosition.X, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z-40);


	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(
		TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	if (P_Explosion.Succeeded())
	{
		Explosion = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_AttackExplosion(
		TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (P_AttackExplosion.Succeeded())
	{
		AttackExplosion = P_AttackExplosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> S_ExplosionSound(
		TEXT("SoundCue'/Game/Sound/Weapons/SMG_Thompson/Cue_Thompson_Shot.Cue_Thompson_Shot'"));
	if (S_ExplosionSound.Succeeded())
	{
		ExplosionSound = S_ExplosionSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim_Dead(
		TEXT("AnimMontage'/Game/Blueprints/Animation/Death_1_Montage.Death_1_Montage'"));
	if (Anim_Dead.Succeeded())
	{
		DeadAnimation = Anim_Dead.Object;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	CurrentHP = MaxHP;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprint && GetCharacterMovement()->Velocity.SizeSquared() == 0)
	{
		bIsSprint = false;
	}
	//UE_LOG(LogClass, Warning, TEXT("%f"), SpringArm->GetComponentLocation().Z);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	
	PlayerInputComponent->BindAction(TEXT("Crouch"),IE_Pressed, this, &AMyCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsights"), IE_Pressed, this, &AMyCharacter::TryIronsight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::UnSprint);

	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Pressed, this, &AMyCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Released, this, &AMyCharacter::UndoLookAround);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::StopFire);


	PlayerInputComponent->BindAction(TEXT("Prone"), IE_Pressed, this, &AMyCharacter::Prone);
	//PlayerInputComponent->BindAction(TEXT("Prone"), IE_Released, this, &AMyCharacter::UnProne);
}

void AMyCharacter::TryIronsight()
{
	if (bIsSprint)
	{
		return;
	}

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
	if (bIsProning)
	{
		return;
	}

	if (CanCrouch() && !bIsSprint && !bIsProne)
	{
		Crouch();
	}
	else if (bIsProne)
	{
		bIsProne = false;
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bIsSprint && !bIsProning)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bIsProning)
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
	// 값만 들어가지 바꿀수없는 값들

	return AimRotLS;
}

void AMyCharacter::Prone()
{
	if (bIsProning)
	{
		return;
	}

	if (!bIsSprint && !bIsProne)
	{
		bIsProne = true;
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed = ProneSpeed;
	}

	else if (bIsProne)
	{
		bIsProne = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
		UnCrouch();
	}
}

void AMyCharacter::Sprint()
{
	if (!bIsCrouched && !bIsIronsight && !bIsFire)
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

void AMyCharacter::StartFire()
{
	if (!bIsSprint && !bIsProning)
	{
		bIsFire = true;
		OnShot();
	}
}

void AMyCharacter::StopFire()
{
	bIsFire = false;
	//GetWorldTimerManager().ClearTimer(TimerHandle);
}


void AMyCharacter::UnSprint()
{
	if(bIsSprint)
	{ 
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	//if (DamageEvent.GetTypeID == FRadialDamageEvent::ClassID)
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
		//RadialDamageEvent->Params.

		UE_LOG(LogClass, Warning, TEXT("RadialDamageEvent 맞았당 : %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{

		UE_LOG(LogClass, Warning, TEXT("DEBUG : %f"), DamageAmount);
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
		//PointDamageEvent->ShotDirection
		//FText -> UI에서 사용
		//FString 은 맨날쓰는 그 문자
		//FName은 내부적을 해싱처리해서 숫자로 비교할떄 사용함
		if (PointDamageEvent->HitInfo.BoneName.Compare(FName(TEXT("head"))) == 0)
		{

			UE_LOG(LogClass, Warning, TEXT("헤드샷 : %f"), DamageAmount);
			CurrentHP = 0;
		}
		else 
		{
			UE_LOG(LogClass, Warning, TEXT("PointDamageEvent 맞았당 : %f, %s"), DamageAmount, *PointDamageEvent->HitInfo.BoneName.ToString());
			CurrentHP -= DamageAmount;
		}

		if (CurrentHP <= 0)
		{
			CurrentHP = 0;
			// GetMesh()->SetSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// DisableInput();
			
			// Montageisplaying
			PlayAnimMontage(DeadAnimation);
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{

	}
	return 0.0f;
}


void AMyCharacter::OnShot()
{
	if (!bIsFire)
	{
		return;
	}
	FTransform WeaponLocation;
	WeaponLocation = Weapon->GetSocketTransform(TEXT("MuzzleFlash"));
	//WeaponLocation = WeaponArrow->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, Weapon->GetSocketTransform(TEXT("MuzzleFlash")), true);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, Weapon->GetSocketTransform(TEXT("MuzzleFlash")).GetLocation());

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);
	// -> 화며8ㄴ 크기 구함.
	
	FVector WorldLocation;
	FVector WorldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->
		DeprojectScreenPositionToWorld(SizeX/2, SizeY/2, WorldLocation, WorldDirection);
	// 화면 좝\표를 월드좌표로 변환.

	//FVector TraceStart =WeaponLocation.GetLocation();
	//FVector TraceEnd =  WeaponLocation.GetLocation() + (WorldDirection * 80000.0f);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (WorldDirection * 80000.0f);

	// 사거리.

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor *> IgnoreObjects;
	// 팀등록 안맞는놈등록/
	FHitResult OutHit;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	IgnoreObjects.Add(this);

	//광선추적시작.
	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false, // 충돌체크를 단순콜리전으로할래 , 복합콜리전으로할래?.
		IgnoreObjects,
		EDrawDebugTrace::ForDuration, // 디버깅용.
		OutHit,
		true,
		FLinearColor::Blue, // 라인색깔.
		FLinearColor::Red, // 맞았을때 라인색깔.
		5.0f
	);

	if (Result)
	{
		TraceStart = Weapon->GetSocketTransform(TEXT("MuzzleFlash")).GetLocation();
		FVector Dir = OutHit.Location - TraceStart;
		TraceEnd = TraceStart + (Dir*2.0f);

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypes,
			false, // 충돌체크를 단순콜리전으로할래 , 복합콜리전으로할래?.
			IgnoreObjects,
			EDrawDebugTrace::ForDuration, // 디버깅용.
			OutHit,
			true,
			FLinearColor::Green, // 라인색깔.
			FLinearColor::Yellow, // 맞았을때 라인색깔.
			5.0f
		);

		if (Result)
		{
			//FTransform HitTransform;
			//HitTransform.SetLocation(OutHit.Location);
			//UGameplayStatics::ApplyDamage(OutHit.GetActor(),
			//	100.0f,
			//	UGameplayStatics::GetPlayerController(GetWorld(), 0),
			//	this,
			//	UBulletDamageType::StaticClass()
			//);
			//

			//UGameplayStatics::ApplyRadialDamage(GetWorld(),
			//	30.0f,
			//	OutHit.ImpactPoint,
			//	1000.0f,
			//	UBulletDamageType::StaticClass(),
			//	IgnoreObjects,
			//	this,
			//	UGameplayStatics::GetPlayerController(GetWorld(),0),
			//	false);

			UGameplayStatics::ApplyPointDamage(
				OutHit.GetActor(),
				30.0f,
				TraceEnd - TraceStart,
				OutHit,
				UGameplayStatics::GetPlayerController(GetWorld(), 0),
				this,
				UBulletDamageType::StaticClass()
			);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackExplosion, OutHit.Location, FRotator::ZeroRotator, true);
		}
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UMyCameraShake::StaticClass());

	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += 1.0f;
	CurrentRotation.Yaw += FMath::RandRange(-0.5f, 0.5f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(CurrentRotation);

	if(bIsFire)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::OnShot, 0.1f);
	}
}