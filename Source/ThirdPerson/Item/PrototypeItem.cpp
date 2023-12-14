// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItem.h"

#include "GameplayTagContainer.h"

UPrototypeItem::UPrototypeItem()
{
	ItemType = FGameplayTag::EmptyTag;
	EquipmentType = FGameplayTag::EmptyTag;
	ItemLevel = 0;
	ItemBaseName = "Default Item";
	ItemFullName = "Default Item";
	ItemStackAmount = 0;
}

void UPrototypeItem::DebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemType: %s, EquipmentType: %s, ItemLevel: %d, ItemName: %s, ItemStackAmount: %d, Rarity: %s"),
		*ItemType.ToString(),
		*EquipmentType.ToString(),
		ItemLevel,
		*ItemBaseName,
		ItemStackAmount,
		*Rarity.ToString()
	);
	for (const auto& Affix : ItemAffixes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Affix: %s"), *Affix->InGameName);
	}
}

bool UPrototypeItem::HasAffixGroupTag(FGameplayTag AffixType, FGameplayTag AffixGroup)
{
	auto AffixGroupTagContainer = AffixGroupTagMap.Find(AffixType);
	if (AffixGroupTagContainer)
	{
		return AffixGroupTagContainer->HasTag(AffixGroup);
	}
	return false;
}
