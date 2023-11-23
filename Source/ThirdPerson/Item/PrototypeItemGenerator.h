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
	FGameplayTag RollItemRarity(FGameplayTag Rarity, int32 MonsterLevel, int32 ItemLevel, FTreasureClass* TreasureClass, bool bIsClassSpecific);
	
	TObjectPtr<UPrototypeItem> GenerateItem_Gold(int32 MonsterLevel);

protected:

private:

	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	TObjectPtr<UDataTable> TreasureClassDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Prototype")
	TObjectPtr<UDataTable> ItemTypeDataTable;
	

	float MagicFindConstant_Unique = 250.0f;
	float MagicFindConstant_Set = 500.0f;
	float MagicFindConstant_Rare = 600.0f;

	/*
	* Itemratio.txt의 Quality, Divisor
	*/
	float Quality_ClassSpecific_Unique = 240.0f;
	float Quality_ClassSpecific_Set = 120.0f;
	float Quality_ClassSpecific_Rare = 80.0f;
	float Quality_ClassSpecific_Magic = 17.0f;
	float Divisor_ClassSpecific_Unique = 3.0f;
	float Divisor_ClassSpecific_Set = 3.0f;
	float Divisor_ClassSpecific_Rare = 3.0f;
	float Divisor_ClassSpecific_Magic = 6.0f;

	float Quality_Unique = 400.0f;
	float Quality_Set = 160.0f;
	float Quality_Rare = 100.0f;
	float Quality_Magic = 34.0f;
	float Divisor_Unique = 1.0f;
	float Divisor_Set = 2.0f;
	float Divisor_Rare = 2.0f;
	float Divisor_Magic = 3.0f;


};
