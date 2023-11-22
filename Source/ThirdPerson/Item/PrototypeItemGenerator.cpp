// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItemGenerator.h"

#include "GameplayTagsManager.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"

#include "ThirdPerson/PrototypeGameplayTags.h"
#include "ThirdPerson/Item/PrototypeItem.h"
#include "ThirdPerson/Game/PrototypeGameModeBase.h"

// Sets default values
UPrototypeItemGenerator::UPrototypeItemGenerator()
{
}

TArray<TObjectPtr<UPrototypeItem>> UPrototypeItemGenerator::GenerateItemsFromTreasureClassTag(FGameplayTag TreasureClassTag, int32 ItemLevel)
{
	const auto& GeneratedItems = GetItemsFromTreasureClassTag(TreasureClassTag, ItemLevel);
	UE_LOG(LogTemp, Warning, TEXT("Generated Items for TreasureClass(%s)"), *TreasureClassTag.ToString());
	for (const auto& Item : GeneratedItems)
	{
		Item->DebugLog();
	}
	return GeneratedItems;
}

TArray<TObjectPtr<UPrototypeItem>> UPrototypeItemGenerator::GetItemsFromTreasureClassTag(FGameplayTag TreasureClassTag, int32 ItemLevel)
{
	TArray<TObjectPtr<UPrototypeItem>> ResultItems;
	const TObjectPtr<APrototypeGameModeBase> GameMode = Cast<APrototypeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemsFromTreasureClassTag: GetGameMode is not valid"));
		return ResultItems;
	}

	const FString Context;
	FTreasureClass* TC = GameMode->TreasureClassDataTable->FindRow<FTreasureClass>(TreasureClassTag.GetTagName(), Context);
	TArray<FTreasure> PickedTreasures;
	// TODO: NumPicks가 음수 일경우 확정적으로 N개를 드랍한다.
	if (TC->NumPicks < 0)
	{

	}
	else
	{
		// TreasureClass에서 TC 또는 아이템을 하나 뽑는다.
		for (int picksLeft = TC->NumPicks; picksLeft > 0; picksLeft--)
		{
			int ProbTotal = TC->FreqNoDrop;
			for (const auto& Treasure : TC->Treasures)
			{
				ProbTotal += Treasure.Probability;
			}

			int RandomProb = FMath::RandRange(0, ProbTotal);
			int ProbRollSum = 0;
			for (const auto& Treasure : TC->Treasures)
			{
				ProbRollSum += Treasure.Probability;
				if (ProbRollSum >= RandomProb)
				{
					PickedTreasures.Add(Treasure);
					break;
				}
			}
		}
	}

	for (const auto& Treasure : PickedTreasures)
	{
		// TreasureClass가 아닌 아이템 타입일 경우 해당 아이템을 드롭
		if (Treasure.Treasure.MatchesTag(TAG_Item_Type))
		{
			TObjectPtr<UPrototypeItem> Item = GetItemFromItemTypeTag(Treasure.Treasure, ItemLevel);
			if (IsValid(Item))
			{
				ResultItems.Add(Item);
			}
		}
		// TreasureClass일 경우 해당 TC로 다시 드랍될 아이템을 뽑는다
		else if (Treasure.Treasure.MatchesTag(TAG_Item_Treasure))
		{
			TArray<TObjectPtr<UPrototypeItem>> Items = GetItemsFromTreasureClassTag(Treasure.Treasure, ItemLevel);
			for (const auto& Item : Items)
			{
				// ApplyDropModifier
				// Init
				ResultItems.Add(Item);
			}
		}
	}

	return ResultItems;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::GetItemFromItemTypeTag(FGameplayTag ItemTypeTag, int32 ItemLevel)
{
	TObjectPtr<UPrototypeItem> ResultItem;
	FGameplayTag PickedEquipmentType;

	if (ItemTypeTag.MatchesTag(TAG_Item_Type_Equipment))
	{
		// TODO: Generic Tag 일 경우, 하위 Tag들을 모두 가져온다.
		// TODO: 하위 태그의 Rarity를 가중치로 활용, 이 중 하나를 뽑는다.
		// TODO: 뽑은 태그 및 아이템 레벨에 맞는 아이템 데이터를 가져온다.
		
		// ItemTypeTag가 Equipment 전체일 경우, Equipment 중에서 하나를 뽑는다.
		if (ItemTypeTag.MatchesTagExact(TAG_Item_Type_Equipment))
		{
			auto TagChildren = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment);
			int RandomIndex = FMath::RandRange(0, TagChildren.Num() - 1);
			PickedEquipmentType = TagChildren.GetByIndex(RandomIndex);
		}
		// ItemTypeTag가 Armor 전체일 경우, Armor 중에서 하나를 뽑는다.
		else if (ItemTypeTag.MatchesTagExact(TAG_Item_Type_Equipment_Armor))
		{
			auto TagChildren = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Armor);
			int RandomIndex = FMath::RandRange(0, TagChildren.Num() - 1);
			PickedEquipmentType = TagChildren.GetByIndex(RandomIndex);
		}
		// ItemTypeTag가 Weapon 전체일 경우, Weapon 중에서 하나를 뽑는다.
		else if (ItemTypeTag.MatchesTagExact(TAG_Item_Type_Equipment_Weapon))
		{
			auto TagChildren = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment_Weapon);
			int RandomIndex = FMath::RandRange(0, TagChildren.Num() - 1);
			PickedEquipmentType = TagChildren.GetByIndex(RandomIndex);
		}
		ResultItem = NewObject<UPrototypeItem>(GetWorld());
		ResultItem->ItemType = ItemTypeTag;
		ResultItem->EquipmentType = PickedEquipmentType;
	}
	else if (ItemTypeTag.MatchesTag(TAG_Item_Type_Gold))
	{
		int32 GoldAmountMin = ItemLevel;
		int32 GoldAmountMax = ItemLevel * 6 - 1;
		// TODO: 정예몬스터면 골드 드랍양이 x배로 늘어난다.
		// TODO: 난이도에 따라서 골드 드랍양이 y배로 늘어난다.
		// TODO: 골드 드랍 +% 스탯에 따라서 골드 드랍양이 늘어난다.
		ResultItem = NewObject<UPrototypeItem>(GetWorld());
		ResultItem->ItemType = ItemTypeTag;
		ResultItem->ItemStackAmount = FMath::RandRange(GoldAmountMin, GoldAmountMax);
	}
	
	return ResultItem;
}

FTreasure UPrototypeItemGenerator::RollTreasurePick(FTreasureClass& TreasureClass)
{
	return FTreasure();
}

