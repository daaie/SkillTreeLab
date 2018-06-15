// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidgetBase.h"
#include "Components/ScrollBox.h"
#include "UI/ItemSlotWidgetBase.h"

void UInventoryWidgetBase::NativeConstruct()
{
	InventoryScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("InventoryScrollBox")));
	if (InventoryScrollBox)
	{
		for (int i = 0; i < InventoryScrollBox->GetChildrenCount(); ++i)
		{
			if (UItemSlotWidgetBase* Slot = Cast<UItemSlotWidgetBase>(InventoryScrollBox->GetChildAt(i)))
			{
				Slots.Add(Slot);
			}
		}
	}

	//for (TFieldIterator<UProperty> PropIt(GetClass()); PropIt; ++PropIt)
	//{
	//	UProperty* Property = *PropIt;
	//}
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetEmptySlot()
{
	for (auto Slot : Slots)
	{
		if (Slot->bIsEmpty)
		{
			return Slot;
		}
	}

	return nullptr;
}

UItemSlotWidgetBase * UInventoryWidgetBase::GetSameIDSlot(int SameItemIndex)
{
	for (auto Slot : Slots)
	{
		if (Slot->Data.ItemIndex == SameItemIndex &&
			!Slot->bIsEmpty &&
			Slot->IsConsume())
		{
			return Slot;
		}
	}
	return nullptr;
}
