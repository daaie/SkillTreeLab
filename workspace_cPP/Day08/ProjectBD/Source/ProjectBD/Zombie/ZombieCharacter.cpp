// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "ZombieTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Basic/MyCharacter.h"
#include "Components/WidgetComponent.h"
#include "ZombieHPBarWidgetBase.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ZOMBIE(TEXT(
		"SkeletalMesh'/Game/Zombie/Mesh/Zombie.Zombie'"));
	if (SK_ZOMBIE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZOMBIE.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT(
		"AnimBlueprint'/Game/Blueprints/Zombie/BP_ZombieAnim.BP_ZombieAnim_C'"));
	if (Anim_Class.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	}

	AIControllerClass = AZombieAIController::StaticClass();
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Zombie(TEXT(
		"BehaviorTree'/Game/Blueprints/Zombie/AI/BT_Zombie.BT_Zombie'"));
	if (BT_Zombie.Succeeded())
	{
		BehaviorTree = BT_Zombie.Object;
	}

	CurrentState = EZombieState::NORMAL;
	CurrentAnimState = EZombieAnimState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 300.0f;
	PawnSensing->SetPeripheralVisionAngle(30.0f);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UZombieHPBarWidgetBase> 
		WC_HPBar(TEXT("WidgetBlueprint'/Game/Blueprints/UI/ZombieHPBarWidget.ZombieHPBarWidget_C'"));
	if (WC_HPBar.Succeeded())
	{
		Widget->SetWidgetClass(WC_HPBar.Class);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_BulletEffect(TEXT("ParticleSystem'/Game/Effects/P_body_bullet_impact.P_body_bullet_impact'"));
	if (P_BulletEffect.Succeeded())
	{
		BulletEffect = P_BulletEffect.Object;
	}

}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;

	TArray<AActor*> OutPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), OutPoints);
	for (auto TargetPoint : OutPoints)
	{
		Targets.Add(Cast<AZombieTargetPoint>(TargetPoint));
	}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AZombieCharacter::OnHearNoise);
	}
}


void AZombieCharacter::OnSeePawn(APawn* Pawn)
{
	AMyCharacter* Player = Cast<AMyCharacter>(Pawn);
	if (Player && Player->CurrentHP > 0 && CurrentState == EZombieState::NORMAL)
	{
		//if(Player->ActorHasTag(""))
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC && AIC->IsValidLowLevel())
		{
			//UE_LOG(LogClass, Warning, TEXT("치즈...곱ㄹ창...맛있따..."));
			CurrentState = EZombieState::CHASE;
			CurrentAnimState = EZombieAnimState::Run;
			AIC->BBComponent->SetValueAsObject(FName(TEXT("Target")), Player);
			AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
	}
}

void AZombieCharacter::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());

	//UE_LOG(LogClass, Warning, TEXT("%s"), *Pawn->GetName());

	FVector Dir = Location - GetActorLocation();
	SetActorRotation(Dir.Rotation());

	if(AIC)
	{
		CurrentState = EZombieState::CHASE;
		AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);

		CurrentState = EZombieState::NORMAL;
		AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
	}

}
// Called every frame
void AZombieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector Dir = CameraLocation - Widget->GetComponentLocation();
	Widget->SetWorldRotation(Dir.Rotation());

	UZombieHPBarWidgetBase* HPBarWidget = Cast<UZombieHPBarWidgetBase>(Widget->GetUserWidgetObject());
	if (HPBarWidget)
	{
		HPBarWidget->Percent = CurrentHP / MaxHP;
	}
}

// Called to bind functionality to input
void AZombieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombieCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (CurrentHP <= 0)
	{
		return 0;
	}

	CurrentState = EZombieState::HIT;
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);


		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletEffect, PointDamageEvent->HitInfo.ImpactPoint,
			FRotator::ZeroRotator);

		if (PointDamageEvent->HitInfo.BoneName.Compare(FName(TEXT("head"))) == 0)
		{
			UE_LOG(LogClass, Warning, TEXT("Head Shot"));
			CurrentHP = 0;
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("Shot"));
			CurrentHP -= Damage;
		}

		if (CurrentHP <= 0)
		{
			CurrentState = EZombieState::DEAD;
			CurrentAnimState = EZombieAnimState::Death;

			AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
			if (AIC && AIC->IsValidLowLevel())
			{
				AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
			}
		

			CurrentHP = 0;
			GetMesh()->SetSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Widget->SetVisibility(false);
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		//DamageEvent
	}
	return 0.0f;
}

void AZombieCharacter::OnAttack()
{
	AZombieAIController *AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AActor* DamageActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Target"))));
		UGameplayStatics::ApplyDamage(DamageActor, Attack, AIC, this, nullptr);

	}
}



