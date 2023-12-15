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
	SocketCount = 0;
}

void UPrototypeItem::DebugLog()
{
	UE_LOG(LogTemp, Warning, TEXT("------ Item Log ------"));
	UE_LOG(LogTemp, Warning, TEXT("ItemType :%s"), *ItemType.ToString());
	UE_LOG(LogTemp, Warning, TEXT("EquipmentType :%s"), *EquipmentType.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ItemLevel :%d"), ItemLevel);
	UE_LOG(LogTemp, Warning, TEXT("ItemFullName :%s"), *ItemFullName);
	UE_LOG(LogTemp, Warning, TEXT("ItemStackAmount :%d"), ItemStackAmount);
	UE_LOG(LogTemp, Warning, TEXT("SocketCount :%d"), SocketCount);
	UE_LOG(LogTemp, Warning, TEXT("Rarity :%s"), *Rarity.ToString());

	for (const auto& Affix : ItemAffixes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Affix: %s"), *Affix->InGameName);
	}
	UE_LOG(LogTemp, Warning, TEXT("------ End of Item Log ------"));
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
