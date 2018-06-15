// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckChase.h"
#include "Zombie/MyZombie.h"
#include "Zombie/ZombieAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Basic/MyCharacter.h"

EBTNodeResult::Type UCheckChase::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMyZombie* Zombie = Cast<AMyZombie>(OwnerComp.GetAIOwner()->GetPawn());

	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	float Range = FVector::Distance(Zombie->GetActorLocation(), Player->GetActorLocation());
	if (Range <= Zombie->AttackRange)
	{
		Zombie->CurrentState = EZombieState::Battle;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;

	}
	else if (Range > Zombie->PawnSensing->SightRadius)
	{
		Zombie->CurrentState = EZombieState::Normal;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

		return EBTNodeResult::Failed;
	}
	else
	{
		// 레이 캐스팅을 해서 시아가 막혀 있으면 돌아감 // 라인 트레이싱
		FVector TraceStart = Zombie->GetActorLocation();
		FVector TraceEnd = Player->GetActorLocation();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> IgnoreObjects;
		FHitResult OutHit;

		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		IgnoreObjects.Add(Zombie);


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

		if (Result)
		{
			if (!OutHit.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				Zombie->CurrentState = EZombieState::Normal;
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("CurrentState")), (uint8)Zombie->CurrentState);

				return EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
