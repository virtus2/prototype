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

void UPrototypeItem::AddAffix(FItemAffix* ItemAffix)
{
	auto Affix = ItemAffixes[0];
	if (Affix)
	{
		auto Modifier = Affix->AffixModifiers[0];
		auto Tag = Modifier.Attribute;
		auto GameplayEffect = ItemAffixAttributes->GetGameplayEffectByAttributeTag(Tag);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GameplayEffect->GetName());
	}
}
