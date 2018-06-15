// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGM.h"
#include "Kismet/GameplayStatics.h"
#include "Battle/ItemPoint.h"
#include "Engine/World.h"
#include "Item/MasterItem.h"
#include "Battle/BattlePC.h"
#include "Battle/BattleGS.h"
#include "Engine/World.h"
#include "TimerManager.h"

void ABattleGM::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemPoint::StaticClass(), ItemPoints);

	FStringClassReference MasterItemClassRef(TEXT("Blueprint'/Game/Blueprints/Item/BP_MasterItem.BP_MasterItem_C'"));
	if (UClass* MyClass = MasterItemClassRef.TryLoadClass<AMasterItem>())
	{
		for (auto ItemPoint : ItemPoints)
		{
			GetWorld()->SpawnActor<AMasterItem>(MyClass, ItemPoint->GetActorLocation(), ItemPoint->GetActorRotation());
		}
	}
}


bool ABattleGM::C2S_CheckAliveCount_Validate()
{
	return true;
}

void ABattleGM::C2S_CheckAliveCount_Implementation()
{
	int TotalCount = 0;
	int AliveCount = 0;

	for (auto i = GetWorld()->GetPlayerControllerIterator(); i; ++i)
	{
		ABattlePC* PC = Cast<ABattlePC>(*i);
		if (PC)
		{
			TotalCount++;
			if (!PC->bIsPawnDead)
			{
				AliveCount++;
			}
		}
	}

	//생존자 수 갱신
	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->AliveCount = AliveCount;
		if (HasAuthority()) //Host
		{
			GS->OnRep_AliveCount(); //Host는 이벤트가 호출 되지 않는다.
		}

		if (GS->AliveCount)
		{
			if (TotalCount != 1 && AliveCount == 1)
			{
				for (auto i = GetWorld()->GetPlayerControllerIterator(); i; ++i)
				{
					ABattlePC* PC = Cast<ABattlePC>(*i);
					if (PC)
					{
						if (!PC->bIsPawnDead)
						{
							UE_LOG(LogClass, Warning, TEXT("UserID : %s"), *PC->GetName());
							//승리화면 출력
							PC->S2C_ConclusionChicken();

							ReturnLobby();
						}
					}
				}
			}
		}
	}
}

void ABattleGM::ReturnLobby()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		this, //콜백 함수가 있는 오브젝트
		&ABattleGM::TimerFunction, //콜백 함수 이름
		1.0f, // 실행 시간
		false, //계속 유지 여부
		2.0f); //첫 실행 시간
}

void ABattleGM::TimerFunction()
{
	LeftTime--;

	ABattleGS* GS = GetGameState<ABattleGS>();
	if (GS)
	{
		GS->PublicMessage = FString::Printf(TEXT("로비로 돌아갑니다...\n%d초 남았습니다."), LeftTime);
		if (HasAuthority()) //서버는 값이 바뀌어도 실행이 안됨
		{
			GS->OnRep_PublicMessage();
		}
	}

	if (LeftTime <= 0) //시작 시간 다 되면 시작
	{
		ABattlePC* PC = Cast<ABattlePC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (PC)
		{
			PC->ReturnLobby(); //로비로 돌아감
		}
	}
	else // 시간 계산 해주기
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			this, //콜백 함수가 있는 오브젝트
			&ABattleGM::TimerFunction, //콜백 함수 이름
			1.0f);
	}
}