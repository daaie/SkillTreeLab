// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Basic/ShootCameraShake.h"
#include "BulletDamageType.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Basic/BasicPC.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Item/MasterItem.h"
#include "UI/ItemTooltipWidgetBase.h"
#include "Components/TextBlock.h"
#include "Item/ItemDataTableComponent.h"
#include "UI/ItemSlotWidgetBase.h"
#include "UI/InventoryWidgetBase.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라 스프링암 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	//스프링 암 위치 조정
	SpringArm->SetRelativeLocation(FVector(0, 30, 70));
	//스프링 암 길이 조정
	SpringArm->TargetArmLength = 150.0f;

	//카메라 생성
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//무기 메시 컴포넌트 생성
	Weapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	//소켓에 연결 하기
	Weapon->SetupAttachment(GetMesh(), TEXT("RHandWeapon"));
	//기본 메시 이동
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));

	//스프링암을 절대 회전으로 회전 시킴
	SpringArm->bUsePawnControlRotation = true;

	//애니메이션을 애니메이션 블루프린트를 사용하는것으로 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//애니메이션 블루프린트 클래스 로딩, 애님 블루프린트명 뒤에 _C
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT("AnimBlueprint'/Game/Blueprints/Player/Animations/ABP_Male.ABP_Male_C'"));
	if (Anim_Class.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	}

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = WalkSpeed;


	NormalSpringArmPosition = SpringArm->GetRelativeTransform().GetLocation();
	CrouchSpringArmPosition = FVector(NormalSpringArmPosition.X, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 40);
	ProneSpringArmPosition = FVector(100, NormalSpringArmPosition.Y, NormalSpringArmPosition.Z - 100);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_MF.P_AssaultRifle_MF'"));
	if (P_Explosion.Succeeded())
	{
		Explosion = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HitEffect(TEXT("ParticleSystem'/Game/Effects/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (P_HitEffect.Succeeded())
	{
		HitEffect = P_HitEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> S_Explosion(TEXT("SoundCue'/Game/Sound/Weapons/SMG_Thompson/Cue_Thompson_Shot.Cue_Thompson_Shot'"));
	if (S_Explosion.Succeeded())
	{
		ExplosionSound = S_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BloodEffect(TEXT("ParticleSystem'/Game/Effects/P_body_bullet_impact.P_body_bullet_impact'"));
	if (P_BloodEffect.Succeeded())
	{
		BloodEffect = P_BloodEffect.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Anim_Dead(TEXT("AnimMontage'/Game/Male_Grunt/Animations/Death_1_Montage.Death_1_Montage'"));
	if (Anim_Dead.Succeeded())
	{
		DeadAnimation = Anim_Dead.Object;
	}

	Tags.Add(FName(TEXT("Player")));

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogClass, Warning, TEXT("%f"), SpringArm->GetComponentLocation().Z);

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
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::TryCrouch);
	PlayerInputComponent->BindAction(TEXT("Ironsight"), IE_Pressed, this, &AMyCharacter::TryIronsight);
	PlayerInputComponent->BindAction(TEXT("Prone"), IE_Pressed, this, &AMyCharacter::TryProne);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::UnSprint);

	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Pressed, this, &AMyCharacter::LookAround);
	PlayerInputComponent->BindAction(TEXT("LookAround"), IE_Released, this, &AMyCharacter::UndoLookAround);


	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyCharacter::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AMyCharacter::StopFire);

	PlayerInputComponent->BindAction(TEXT("ItemGet"), IE_Released, this, &AMyCharacter::ItemGet);
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Released, this, &AMyCharacter::Inventory);

}

void AMyCharacter::ItemGet()
{
	AMasterItem* PickupItem = GetClosestItem();
	if (PickupItem && !PickupItem->IsPendingKill())
	{
		RemovePickupItemList(PickupItem);
		ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PC)
		{
			UItemSlotWidgetBase* Slot = PC->InventoryWidget->GetSameIDSlot(PickupItem->ItemIndex);
			if (Slot)
			{
				Slot->AddItemCount(PickupItem->ItemCount);
				PickupItem->Destroy();
				ViewItemTooltip();
			}
			else
			{
				UItemSlotWidgetBase* Slot = PC->InventoryWidget->GetEmptySlot();
				if (Slot)
				{
					Slot->SetItem(PickupItem->ItemIndex);
					PickupItem->Destroy();
					ViewItemTooltip();
				}
				else
				{
					UE_LOG(LogClass, Warning, TEXT("Inventory full."))
				}
			}
		}
	}
}

void AMyCharacter::Inventory()
{
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->ToggleInventory();
	}
}

AMasterItem* AMyCharacter::GetClosestItem()
{
	AMasterItem* ClosestItem = nullptr;
	float Min = 9999999999.9f;

	for (auto Item : CanPickupList)
	{
		float Distance = FVector::Distance(Item->GetActorLocation(), GetActorLocation());
		if (Min > Distance)
		{
			Min = Distance;
			ClosestItem = Item;
		}
	}

	return ClosestItem;
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
	if (bIsProning) //엎드리는 중에는 상태 변화 안됨
	{
		return;
	}

	if (CanCrouch() && !bIsSprint && !bIsProne) //서있을때
	{
		Crouch();
	}
	else if (bIsProne) //기어 다닐때
	{
		bIsProne = false;
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMyCharacter::Turn(float Value)
{
	if (Value != 0.0)
	{
		AddControllerYawInput(Value);
	}
}

void AMyCharacter::LookUp(float Value)
{
	if (Value != 0.0)
	{
		AddControllerPitchInput(-Value);
	}
}

void AMyCharacter::MoveRight(float Value)
{
	if (Value != 0.0 && !bIsSprint && !bIsProning)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void AMyCharacter::MoveForward(float Value)
{
	if (Value != 0.0 && !bIsProning)
	{
		AddMovementInput(GetActorForwardVector(), Value);
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
	if (!bIsCrouched && !bIsIronsight &&  !bIsProne && !bIsProning &&
		!bIsFire && GetCharacterMovement()->Velocity.Size() > 0)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AMyCharacter::UnSprint()
{
	if (bIsSprint)
	{
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
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

void AMyCharacter::TryProne()
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
	//GetWorldTimerManager().ClearTimer(ShootTimeHandle);
}

void AMyCharacter::OnShot()
{
	if (!bIsFire)
	{
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	int SizeX;
	int SizeY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetViewportSize(SizeX, SizeY);

	FVector WorldLocation;
	FVector WorldDirection;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectScreenPositionToWorld(SizeX / 2, SizeY / 2, WorldLocation, WorldDirection);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (WorldDirection * 80000.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreObjects;
	FHitResult OutHit;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	IgnoreObjects.Add(this);


	//광선 추적 시작
	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false,
		IgnoreObjects,
		EDrawDebugTrace::None,
		OutHit,
		true,
		FLinearColor::Blue,
		FLinearColor::Red,
		5.0f);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, Weapon->GetSocketTransform(TEXT("MuzzleFlash")));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, Weapon->GetSocketTransform(TEXT("MuzzleFlash")).GetLocation());

	if (Result)
	{
		TraceStart = Weapon->GetSocketTransform(TEXT("MuzzleFlash")).GetLocation();
		FVector Dir = OutHit.Location - TraceStart;
		TraceEnd = TraceStart + (Dir * 2.0f);

		bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypes,
			false,
			IgnoreObjects,
			EDrawDebugTrace::None,
			OutHit,
			true,
			FLinearColor::Green,
			FLinearColor::Green,
			5.0f);
		if (Result)
		{
			//UGameplayStatics::ApplyDamage(OutHit.GetActor(),
			//	0.0f,
			//	UGameplayStatics::GetPlayerController(GetWorld(), 0),
			//	this,
			//	UBulletDamageType::StaticClass()
			//);

			//UGameplayStatics::ApplyRadialDamage(GetWorld(),
			//	30.0f,
			//	OutHit.ImpactPoint,
			//	300.0f,
			//	UBulletDamageType::StaticClass(),
			//	IgnoreObjects,
			//	this,
			//	UGameplayStatics::GetPlayerController(GetWorld(), 0),
			//	false);

			UGameplayStatics::ApplyPointDamage(OutHit.GetActor(),
				30.0f,
				TraceEnd - TraceStart,
				OutHit,
				UGameplayStatics::GetPlayerController(GetWorld(), 0),
				this,
				UBulletDamageType::StaticClass()
			);

			APawn* Pawn = Cast<APawn>(OutHit.GetActor());
			if (Pawn)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OutHit.Location,FRotator::ZeroRotator);
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, OutHit.Location,					FRotator::ZeroRotator);
			}

			NoiseEmitter->MakeNoise(this, 1.0f, OutHit.Location);
			NoiseEmitter->NoiseLifetime = 0.2f;
		}
	}

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->PlayCameraShake(UShootCameraShake::StaticClass());

	FRotator CurrentRotation = GetControlRotation();
	CurrentRotation.Pitch += 1.0f;
	CurrentRotation.Yaw += FMath::RandRange(-0.5f, 0.5f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(CurrentRotation);

	if (bIsFire)
	{
		GetWorldTimerManager().SetTimer(ShootTimeHandle, this, &AMyCharacter::OnShot, 0.2f);
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return 0;
	}

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		if (PointDamageEvent->HitInfo.BoneName.Compare(FName(TEXT("head"))) == 0)
		{
			UE_LOG(LogClass, Warning, TEXT("Head Shot") );
			CurrentHP = 0;
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("Shot"));
			CurrentHP -= DamageAmount;
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
	}

	if (CurrentHP <= 0)
	{
		CurrentHP = 0;
		//GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DisableInput(Cast<ABasicPC>(GetController()));

		PlayAnimMontage(DeadAnimation);
	}

	//Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void AMyCharacter::AddPickupItemList(AMasterItem * Item)
{
	if (Item && !Item->IsPendingKill())
	{
		CanPickupList.Add(Item);
	}

	ViewItemTooltip();
}

void AMyCharacter::RemovePickupItemList(AMasterItem * Item)
{
	if (Item)
	{
		CanPickupList.Remove(Item);
	}

	ViewItemTooltip();
}

void AMyCharacter::ViewItemTooltip()
{
	ABasicPC* PC = Cast<ABasicPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	if (CanPickupList.Num() == 0)
	{
		UE_LOG(LogClass, Warning, TEXT("ViewToopTip"));

		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	//젤 가까운 아이템 가져오기
	AMasterItem* ClosestItem = GetClosestItem();
	if (ClosestItem)
	{
		PC->ItemTooltip->ItemName->SetText(FText::FromString(ClosestItem->ItemDataTable->GetItemData(ClosestItem->ItemIndex).ItemName));
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PC->ItemTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}
