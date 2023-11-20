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

	TArray<TObjectPtr<UPrototypeItem>> ItemsFromTreasureClassTag(FGameplayTag TreasureClassTag);
	TObjectPtr<UPrototypeItem> ItemFromItemTypeTag(FGameplayTag ItemTypeTag);
	FTreasure RollTreasurePick(FTreasureClass& TreasureClass);

protected:

private:

};
