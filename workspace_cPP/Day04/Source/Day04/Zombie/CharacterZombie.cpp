// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterZombie.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ZombieAIController.h"
#include "MyTargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterZombie::ACharacterZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ZOMBIE(TEXT(
		"SkeletalMesh'/Game/Zombie/Mesh/Zombie.Zombie'"));
	if (SK_ZOMBIE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_ZOMBIE.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	FStringClassReference Anim_Zombie(TEXT("AnimBlueprint'/Game/Blueprints/Zombie/BP_AnimZombie.BP_AnimZombie_C'"));
	if (UClass* Anim_Zombie_Class = Anim_Zombie.TryLoadClass<UAnimInstance>())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Zombie_Class);
	}

	//오브젝트 레퍼런스
	//AZombieAIController *ObjectRef = NewObject<AZombieAIController>();
	
	// 클래스 레퍼런스.
	AIControllerClass = AZombieAIController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Zombie(TEXT(
		"BehaviorTree'/Game/Blueprints/Zombie/AI/BT_Zombie.BT_Zombie'"));
	if (BT_Zombie.Succeeded())
	{
		BehaviorTree = BT_Zombie.Object;
	}


	CurrentState = EZombieState::NORMAL;
	CurrentAnimState = EZombieAnimState::Idle;
}

// Called when the game starts or when spawned
void ACharacterZombie::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHP = MaxHP;

	TArray<AActor*> Outer;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyTargetPoint::StaticClass(), Outer);
	for (auto PatrolPoint : Outer)
	//for (AActor* PatrolPoint : Outer)
	{
		PatrolPoints.Add(Cast<AMyTargetPoint>(PatrolPoint));
	}

	//for (int i = 0; i < Outer.Num(); i++)
	//{
	//	PatrolPoints.Add(Cast<AMyTargetPoint>(Outer[i]));
	//}
}

// Called every frame
void ACharacterZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ACharacterZombie::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* RadialDamageEvent = (FRadialDamageEvent*)(&DamageEvent);

		UE_LOG(LogClass, Warning, TEXT("RadialDamageEvent 맞았당 : %f"), DamageAmount);
	}
	else if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);
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
			GetMesh()->SetSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		}
	}
	else if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{

	}
	return 0.0f;
}
