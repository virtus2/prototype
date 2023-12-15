// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItem.h"

#include "GameplayTagContainer.h"

UPrototypeItem::UPrototypeItem()
{
	ItemType = FGameplayTag::EmptyTag;
	EquipmentType = FGameplayTag::EmptyTag;
	ItemLevel = 0;
	LevelRequirement = 0;
	ItemBaseName = "Default Item";
	ItemFullName = "Default Item";
	ItemStackAmount = 0;
}

void UPrototypeItem::DebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemType: %s, EquipmentType: %s, ItemLevel: %d, ItemFullName: %s, ItemStackAmount: %d, Rarity: %s"),
		*ItemType.ToString(),
		*EquipmentType.ToString(),
		ItemLevel,
		*ItemFullName,
		ItemStackAmount,
		*Rarity.ToString()
	);
	for (const auto& Affix : ItemAffixes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Affix: %s"), *Affix->InGameName);
	}
}

void UPrototypeItem::AddAffix(FItemAffix* ItemAffixData)
{
	ItemAffixes.Add(ItemAffixData);
	// 같은 그룹의 접사가 또 붙는걸 방지하기 위해서 아이템에 접사의 그룹을 모두 추가해놓는다.
	AffixGroupTagContainer.AddTag(ItemAffixData->AffixGroup);
}

bool UPrototypeItem::HasAffixGroupTag(FGameplayTag AffixType, FGameplayTag AffixGroup)
{
	return AffixGroupTagContainer.HasTag(AffixGroup);
}
