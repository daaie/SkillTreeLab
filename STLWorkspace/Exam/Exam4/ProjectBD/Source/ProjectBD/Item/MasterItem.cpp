// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemDataTableComponent.h"
#include "Engine/StreamableManager.h"
#include "Basic/MyCharacter.h"

AMasterItem::AMasterItem()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetStaticMeshComponent());
	Sphere->SetSphereRadius(200.0f);
	Sphere->bGenerateOverlapEvents = true;

	ItemDataTableComponent = CreateDefaultSubobject<UItemDataTableComponent>(TEXT("ItemDataTable"));
}

void AMasterItem::BeginPlay()
{
	Super::BeginPlay();
	if (ItemDataTableComponent && ItemDataTableComponent->DataTable)
	{
		ItemIndex = FMath::RandRange(1, 6) * 10;
		FItemDataTable data = ItemDataTableComponent->GetItemData(ItemIndex);
		ItemCount = data.ItemCount;

		FStreamableManager AssetLoader;
		GetStaticMeshComponent()->SetStaticMesh(AssetLoader.LoadSynchronous<UStaticMesh>(data.ItemMesh));

		Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMasterItem::OnBeginOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &AMasterItem::OnEndOverlap);
	}
}

void AMasterItem::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(OtherActor);
		if (Pawn)
		{
			Pawn->AddPickupItemList(this);
		}
	}
}

void AMasterItem::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		AMyCharacter* Pawn = Cast<AMyCharacter>(OtherActor);
		if (Pawn)
		{
			Pawn->RemovePickupItemList(this);
		}
	}
}
