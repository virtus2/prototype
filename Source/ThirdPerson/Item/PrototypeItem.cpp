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
	// ���� �׷��� ���簡 �� �ٴ°� �����ϱ� ���ؼ� �����ۿ� ������ �׷��� ��� �߰��س��´�.
	AffixGroupTagContainer.AddTag(ItemAffixData->AffixGroup);
}

bool UPrototypeItem::HasAffixGroupTag(FGameplayTag AffixType, FGameplayTag AffixGroup)
{
	return AffixGroupTagContainer.HasTag(AffixGroup);
}
