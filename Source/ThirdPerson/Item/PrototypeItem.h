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

	void AddAffix(FItemAffix* ItemAffix);

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
	FName ItemName;

	UPROPERTY(VisibleAnywhere)
	int32 ItemStackAmount;

protected:
	

private:

	UPROPERTY(VisibleAnywhere)
	TArray<FItemAffix*> ItemAffixes;
	
};
