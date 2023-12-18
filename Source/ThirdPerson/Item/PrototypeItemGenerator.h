// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ThirdPerson/Ability/AttributeGameplayEffectMap.h"
#include "ThirdPerson/Data/TreasureClass.h"
#include "ThirdPerson/Data/ItemAffix.h"
#include "ThirdPerson/Data/Armor.h"
#include "ThirdPerson/Data/Weapon.h"

#include "PrototypeItemGenerator.generated.h"

class UDataTable;
class UAttributeGameplayEffectMap;
class UPrototypeItem;
struct FTreasureClass;
struct FGamepalyTag;
struct FItemType;
struct FArmor;
struct FWeapon;
struct FTableRowBase;

UCLASS()
class THIRDPERSON_API UPrototypeItemGenerator : public UObject
{
	GENERATED_BODY()
	
public:	
	UPrototypeItemGenerator();

	void GenerateItems(FGameplayTag TreasureClass, int32 MonsterLevel, /* out */ TArray<TObjectPtr<UPrototypeItem>>& GeneratedItems);
	void SpawnItemsAt(FGameplayTag TreasureClass, int32 MonsterLevel, FVector Location);
protected:

private:
	TArray<FGameplayTag> RollTreasureClassPicks(FTreasureClass* TreasureClass, int32 MonsterLevel);

	FGameplayTag PickItemType(FGameplayTag ItemType, int32 MonsterLevel);
    FGameplayTag PickItemType_Equipment(FGameplayTag ItemType);

	TObjectPtr<UPrototypeItem> GenerateItem_Gold(int32 MonsterLevel);
	TObjectPtr<UPrototypeItem> GenerateItem_Equipment(FGameplayTag ItemType, int32 MonsterLevel, FTreasureClass* TreasureClass, FItemType* ItemTypeData);

	FGameplayTag RollItemRarity(int32 MonsterLevel, int32 ItemLevel, FTreasureClass* TreasureClass, bool bIsClassSpecificItem);
	bool CheckItemRarityCalc(int32 MonsterLevel, int32 ItemLevel, int32 Quality, int32 Divisor, float MagicFindConst, int32 FreqRarity);

	void RollItemAffixes(TObjectPtr<UPrototypeItem> Item);
	void AddItemAffixes(TObjectPtr<UPrototypeItem> Item, FGameplayTag AffixType, int32 AffixCount);

protected:

private:

	UPROPERTY()
	TObjectPtr<UDataTable> TreasureClassDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> ItemTypeDataTable;


	/* Item Affix Data */
	UPROPERTY()
	TObjectPtr<UAttributeGameplayEffectMap> ItemAffixAttributes;

	UPROPERTY()
	TObjectPtr<UDataTable> ItemAffixDataTable;
	TArray<FItemAffix*> AllItemAffixes;
	TMap<FGameplayTag, TArray<FItemAffix*>> AffixTypeToAffixData;
	/* End of Item Affix Data */


	/* Equipment Item Data */
	UPROPERTY()
	TObjectPtr<UDataTable> ArmorDataTable;
	TArray<FArmor*> AllArmorData;
	TMap<FGameplayTag, TArray<FArmor*>> ItemTypeToArmorData;

	UPROPERTY()
	TObjectPtr<UDataTable> WeaponDataTable;
	TArray<FWeapon*> AllWeaponData;
	TMap<FGameplayTag, TArray<FWeapon*>> ItemTypeToWeaponData;
	/* End of Equipment Item Data */

	

	/*
	* 
	*/
	float MagicFindConstant_Unique = 250.0f;
	float MagicFindConstant_Set = 500.0f;
	float MagicFindConstant_Rare = 600.0f;
	float MagicFindConstant_Magic = 1024.0f;

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
