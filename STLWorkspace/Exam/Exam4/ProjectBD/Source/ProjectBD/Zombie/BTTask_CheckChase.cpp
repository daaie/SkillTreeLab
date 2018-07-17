// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CheckChase.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Basic/MyCharacter.h"


EBTNodeResult::Type UBTTask_CheckChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieCharacter* Zombie = Cast<AZombieCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	FVector TraceStart = Zombie->GetActorLocation();
	FVector TraceEnd = Player->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> IgnoreObjects;
	FHitResult OutHit;

	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

	IgnoreObjects.Add(Player);

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false,
		IgnoreObjects,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Blue,
		FLinearColor::Red,
		5.0f);

	float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
	if (Range <= Zombie->AttackRange)
	{
		Zombie->CurrentState = EZombieState::BATTLE;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;
	}
	else if (Range > Zombie->PawnSensing->SightRadius)
	{
		Zombie->CurrentState = EZombieState::NORMAL;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;
	}
	// 레이 캐스팅을 해서 시야가 막혀있으면 돌아감 // 라인 트레이싱.
	if (Result)
	{
		if (!OutHit.GetActor()->ActorHasTag(FName(TEXT("Player"))))
		{
			UE_LOG(LogClass, Warning, TEXT("디버그ㅡㅡㅡㅡ"));
			Zombie->CurrentState = EZombieState::NORMAL;
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}