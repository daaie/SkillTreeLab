// Fill out your copyright notice in the Description page of Project Settings.

#include "MyZombie.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Zombie/PatrolPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Basic/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Zombie/UI/ZombieHpBarWidgetBase.h"


// Sets default values
AMyZombie::AMyZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//애니메이션을 애니메이션 블루프린트를 사용하는것으로 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Zombie(TEXT("SkeletalMesh'/Game/Zombie/Mesh/Zombie.Zombie'"));
	if (SK_Zombie.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Zombie.Object);
	}

	//애니메이션 블루프린트 클래스 로딩, 애님 블루프린트명 뒤에 _C
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Class(TEXT("AnimBlueprint'/Game/Blueprints/Zombie/ABP_Zombie.ABP_Zombie_C'"));
	if (Anim_Class.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Class.Class);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));

	AZombieAIController* P = NewObject<AZombieAIController>();

	AIControllerClass = AZombieAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Zombie(TEXT("BehaviorTree'/Game/Blueprints/Zombie/AI/BT_Zombie.BT_Zombie'"));
	if (BT_Zombie.Succeeded())
	{
		BehaviorTree = BT_Zombie.Object;
	}

	CurrentState = EZombieState::Normal;
	CurrentAnimState = EZombieAnimState::Idle;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 300.0f;
	PawnSensing->SetPeripheralVisionAngle(60.0);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UZombieHpBarWidgetBase> Widget_Class(TEXT("WidgetBlueprint'/Game/Blueprints/Zombie/UI/ZombieHPBarWidget.ZombieHPBarWidget_C'"));
	if (Widget_Class.Succeeded())
	{
		Widget->SetWidgetClass(Widget_Class.Class);
	}

}

// Called when the game starts or when spawned
void AMyZombie::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

	TArray<AActor*> Outer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), Outer);
	//for (AActor* PatrolPoint : Outer)
	for (auto PatrolPoint : Outer)
	{
		PatrolPoints.Add(Cast<APatrolPoint>(PatrolPoint));
	}

	//for (int i = 0; i < Outer.Num(); i++)
	//{
	//	PatrolPoints.Add(Cast<APatrolPoint>(Outer[i]));
	//}

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AMyZombie::OnSeePawn);
		PawnSensing->OnHearNoise.AddDynamic(this, &AMyZombie::OnHearNoise);
	}
}

void AMyZombie::OnSeePawn(APawn* Pawn)
{
	AMyCharacter* Player = Cast<AMyCharacter>(Pawn);

	if (Player && Player->CurrentHP > 0 && CurrentState == EZombieState::Normal)
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			CurrentState = EZombieState::Chase;
			CurrentAnimState = EZombieAnimState::Run;

			AIC->BBComponent->SetValueAsObject(FName(TEXT("Target")), Player);
			AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
	}
}

void AMyZombie::OnHearNoise(APawn* InPawn, const FVector& Location, float Volume)
{
	AMyCharacter* Player = Cast<AMyCharacter>(InPawn);

	if (Player && Player->CurrentHP > 0 && CurrentState == EZombieState::Normal)
	{
		AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
		if (AIC)
		{
			CurrentState = EZombieState::Chase;
			CurrentAnimState = EZombieAnimState::Run;

			FVector Dir = Player->GetActorLocation() - GetActorLocation();
			SetActorRotation(Dir.Rotation());

			AIC->BBComponent->SetValueAsObject(FName(TEXT("Target")), Player);
			AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)CurrentState);
		}
	}
}

// Called every frame
void AMyZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CameraLocation;
	FRotator CameraRotation;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPlayerViewPoint(CameraLocation, CameraRotation);
	
	FVector Dir = CameraLocation - Widget->GetComponentLocation();
	Widget->SetWorldRotation(Dir.Rotation());

	UZombieHpBarWidgetBase* HPBarWidget = Cast<UZombieHpBarWidgetBase>(Widget->GetUserWidgetObject());
	if (HPBarWidget)
	{
		HPBarWidget->Percent = CurrentHP / MaxHP;
	}

}

// Called to bind functionality to input
void AMyZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
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
			UE_LOG(LogClass, Warning, TEXT("Head Shot"));
			CurrentHP = 0;
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("Shot"));
			CurrentHP -= DamageAmount;
		}

		if (CurrentHP <= 0)
		{
			CurrentState = EZombieState::Dead;
			CurrentAnimState = EZombieAnimState::Death;
			AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
			if (AIC)
			{
				AIC->BBComponent->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)
					CurrentState);
			}


			CurrentHP = 0;
			GetMesh()->SetSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			Widget->SetVisibility(false);
		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		//DamageEvent
	}

	return 0.0f;
}

void AMyZombie::OnAttack()
{
	AZombieAIController* AIC = Cast<AZombieAIController>(GetController());
	if (AIC)
	{
		AActor* DamageActor = Cast<AActor>(AIC->BBComponent->GetValueAsObject(FName(TEXT("Target"))));
		UGameplayStatics::ApplyDamage(DamageActor, Attack, AIC, this, nullptr);
	}
}
