// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItem.h"

#include "GameplayTagContainer.h"

UPrototypeItem::UPrototypeItem()
{
	ItemType = FGameplayTag::EmptyTag;
	EquipmentType = FGameplayTag::EmptyTag;
	ItemLevel = 0;
	ItemName = FName();
	ItemStackAmount = 0;
}

void UPrototypeItem::DebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemType: %s, EquipmentType: %s, ItemLevel: %d, ItemName: %s, ItemStackAmount: %d, Rarity: %s"),
		*ItemType.ToString(),
		*EquipmentType.ToString(),
		ItemLevel,
		*ItemName.ToString(),
		ItemStackAmount,
		*Rarity.ToString()
	);
}
