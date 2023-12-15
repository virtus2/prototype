// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "ThirdPerson/Data/ItemAffix.h"

#include "PrototypeItem.generated.h"


/**
 * 
 */
UCLASS()
class THIRDPERSON_API UPrototypeItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPrototypeItem();

	virtual void DebugLog();

	void AddAffix(FItemAffix* ItemAffixData);
	bool HasAffixGroupTag(FGameplayTag AffixType, FGameplayTag AffixGroup);

protected:
	

private:

public:
	UPROPERTY(VisibleAnywhere)
	FGameplayTag ItemType;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag Rarity;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag EquipmentType;

	UPROPERTY(VisibleAnywhere)
	int ItemLevel;

	UPROPERTY(VisibleAnywhere)
	int32 LevelRequirement;

	/* 접사까지 포함한 아이템의 풀 네임 */
	UPROPERTY(VisibleAnywhere)
	FString ItemFullName;

	/* 베이스 아이템의 이름 */
	UPROPERTY(VisibleAnywhere)
	FString ItemBaseName;

	UPROPERTY(VisibleAnywhere)
	int32 ItemStackAmount;

	/*
	* Equipment Item Related Attributes
	* 추후에 child class로 분리할수도 있음...
	*/
	UPROPERTY(VisibleAnywhere)
	int32 Defense;

	UPROPERTY(VisibleAnywhere)
	int32 SocketCount;
	/* End of Equipment Item Related Attributes */



	TArray<FItemAffix*> ItemAffixes;
	FGameplayTagContainer AffixGroupTagContainer;
protected:
	

private:

	
};
