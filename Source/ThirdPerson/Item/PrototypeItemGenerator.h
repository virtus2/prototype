// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ThirdPerson/Data/TreasureClass.h"

#include "PrototypeItemGenerator.generated.h"

class UPrototypeItem;

UCLASS()
class THIRDPERSON_API UPrototypeItemGenerator : public UObject
{
	GENERATED_BODY()
	
public:	
	UPrototypeItemGenerator();

	TArray<TObjectPtr<UPrototypeItem>> GenerateItems(FGameplayTag TreasureClass, int32 MonsterLevel);
protected:

private:
	TArray<TObjectPtr<UPrototypeItem>> RollTreasureClassPicks(FGameplayTag TreasureClassTag, int32 MonsterLevel);

	TObjectPtr<UPrototypeItem> PickItem(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass);
	TObjectPtr<UPrototypeItem> PickItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass);
	TObjectPtr<UPrototypeItem> PickItem_Gold(int32 MonsterLevel);

	TObjectPtr<UPrototypeItem> GenerateItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass);
	FGameplayTag RollItemRarity(FGameplayTag Rarity, int32 MonsterLevel, int32 ItemLevel, FTreasureClass* TreasureClass);
	TObjectPtr<UPrototypeItem> GenerateItem_Gold(int32 MonsterLevel);

protected:

private:

	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	TObjectPtr<UDataTable> TreasureClassDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	TObjectPtr<UDataTable> ItemTypeDataTable;

};
