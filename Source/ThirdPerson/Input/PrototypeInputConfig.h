// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PrototypeInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FPrototypeInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const  UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};


UCLASS()
class THIRDPERSON_API UPrototypeInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FPrototypeInputAction> AbilityInputActions;
	
};
