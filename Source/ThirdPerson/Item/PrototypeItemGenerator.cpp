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

TArray<TObjectPtr<UPrototypeItem>> UPrototypeItemGenerator::ItemsFromTreasureClassTag(FGameplayTag TreasureClassTag)
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
	// TODO: 음수 일경우 확정적으로 N개를 드랍한다.
	if (TC->NumPicks < 0)
	{

	}
	else
	{
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
		if (Treasure.Treasure.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Type")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item.Type"));
			TObjectPtr<UPrototypeItem> Item = ItemFromItemTypeTag(Treasure.Treasure);
			if (IsValid(Item))
			{
				UE_LOG(LogTemp, Warning, TEXT("Item.Type1"));
				ResultItems.Add(Item);
			}
		}
		// TreasureClass일 경우 해당 TC로 다시 드랍될 아이템을 뽑는다
		else if (Treasure.Treasure.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Treasure")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item.Treasure"));
			TArray<TObjectPtr<UPrototypeItem>> Items = ItemsFromTreasureClassTag(Treasure.Treasure);
			for (const auto& Item : Items)
			{
				UE_LOG(LogTemp, Warning, TEXT("Item.Treasure1"));
				// ApplyDropModifier
				// Init
				ResultItems.Add(Item);
			}
		}
	}
	return ResultItems;
}

TObjectPtr<UPrototypeItem> UPrototypeItemGenerator::ItemFromItemTypeTag(FGameplayTag ItemTypeTag)
{
	TObjectPtr<UPrototypeItem> ResultItem;
	FGameplayTag PickedEquipmentType; // 추첨한 

	// FGameplayTag ItemGemTag = FGameplayTag::RequestGameplayTag("Item.Type.Gem");

	// ItemTypeTag가 Equipment 전체일 경우, Equipment 중에서 하나를 뽑는다.
	if (ItemTypeTag.MatchesTagExact(TAG_Item_Type_Equipment))
	{
		auto EquipmentTagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(TAG_Item_Type_Equipment);
		for (const auto& Tag : EquipmentTagContainer)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Tag.ToString());
		}
		int RandomIndex = FMath::RandRange(0, EquipmentTagContainer.Num() - 1);
		PickedEquipmentType = EquipmentTagContainer.GetByIndex(RandomIndex);
	}
	ResultItem = NewObject<UPrototypeItem>(GetWorld());
	ResultItem->ItemType = ItemTypeTag;
	ResultItem->EquipmentType = PickedEquipmentType;
	UE_LOG(LogTemp, Warning, TEXT("ResultItem->ItemType: %s"), *ResultItem->ItemType.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ResultItem->EquipmentType: %s"), *ResultItem->EquipmentType.ToString());
	
	return ResultItem;
}

FTreasure UPrototypeItemGenerator::RollTreasurePick(FTreasureClass& TreasureClass)
{
	return FTreasure();
}

