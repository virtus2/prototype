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

	TArray<TObjectPtr<UPrototypeItem>> GenerateItemsFromTreasureClassTag(FGameplayTag TreasureClassTag, int32 ItemLevel);
	TArray<TObjectPtr<UPrototypeItem>> GetItemsFromTreasureClassTag(FGameplayTag TreasureClassTag, int32 ItemLevel);
	TObjectPtr<UPrototypeItem> GetItemFromItemTypeTag(FGameplayTag ItemTypeTag, int32 ItemLevel);
	FTreasure RollTreasurePick(FTreasureClass& TreasureClass);

protected:

private:

};
