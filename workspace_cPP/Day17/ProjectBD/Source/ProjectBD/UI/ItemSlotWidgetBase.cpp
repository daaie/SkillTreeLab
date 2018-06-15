// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotWidgetBase.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Basic/MyCharacter.h"
#include "Basic/BasicPC.h"


void UItemSlotWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	FStreamableManager AssetLoader;
	DataTable = AssetLoader.LoadSynchronous<UDataTable>(FStringAssetReference(TEXT("DataTable'/Game/Blueprints/Data/ItemTable.ItemTable'")));

	ItemThumb = Cast<UImage>(GetWidgetFromName(TEXT("ItemThumb")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	ItemCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UItemSlotWidgetBase::UseItem);
	}
}

void UItemSlotWidgetBase::SetItem(int NewItemIndex)
{
	Data.ItemIndex = NewItemIndex;
	if (GetItemTableRow(Data))
	{
		if (ItemThumb)
		{
			FStreamableManager AssetLoader;
			ItemThumb->SetBrushFromTexture(AssetLoader.LoadSynchronous<UTexture2D>(Data.ItemThumnail));
		}

		if (ItemName)
		{
			ItemName->SetText(FText::FromString(Data.ItemName));
		}

		if (ItemCount)
		{
			ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
		}

		bIsEmpty = false;

		SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemSlotWidgetBase::ClearItem()
{
	bIsEmpty = true;
	SetVisibility(ESlateVisibility::Collapsed);
}

bool UItemSlotWidgetBase::IsConsume()
{
	return (Data.ItemType == EItemType::Consume);
}

void UItemSlotWidgetBase::AddItemCount(int AddCount)
{
	Data.ItemCount += AddCount;
	if (ItemCount)
	{
		ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
	}
}

bool UItemSlotWidgetBase::GetItemTableRow(FItemDataTable & Data)
{
	if (DataTable)
	{
		if (Data.ItemIndex != 0)
		{
			Data = *DataTable->FindRow<FItemDataTable>(FName(*FString::FromInt(Data.ItemIndex)), TEXT("ItemIndex"));

			if (&Data == nullptr)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

FReply UItemSlotWidgetBase::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && !bIsEmpty)
	{
		UseItem();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UItemSlotWidgetBase::UseItem()
{
	if (IsConsume())
	{
		Data.ItemCount--;

		AMyCharacter* Pawn = Cast<AMyCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
		if (Pawn)
		{
			Pawn->CurrentHP += Data.ItemFloat01;
			Pawn->CurrentHP = FMath::Clamp<float>(Pawn->CurrentHP, 0, Pawn->MaxHP);
		}

		if (Data.ItemCount == 0)
		{
			ClearItem();
		}
		else
		{
			if (ItemCount)
			{
				ItemCount->SetText(FText::FromString(FString::FromInt(Data.ItemCount)));
			}
		}
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Equip."));
	}

}
