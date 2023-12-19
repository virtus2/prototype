// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ItemFactory.h"

#include "GoldFactory.generated.h"
/**
 * 
 */
UCLASS()
class THIRDPERSON_API UGoldFactory : public UObject, public IItemFactory
{
	GENERATED_BODY()

	virtual void CreateItems(TArray<TObjectPtr<UPrototypeItem>>& OutItems) override;
};
