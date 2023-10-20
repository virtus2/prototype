// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PrototypeGameplayAbility.generated.h"

struct FGameplayTag;

UCLASS()
class THIRDPERSON_API UPrototypeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class UPrototypeAbilitySystemComponent;

public:
	UPrototypeGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// TODO: 바인딩할 UInputAction 추가?
	UPROPERTY(EditDefaultsOnly, Category = "Prototype|Input")
	FGameplayTag StartupInputTag;

	// TODO: Booleans / Bitfields for core checks that we don't want to add tags to every ability for.
	// Examples
	// - Activate On Granted
	// - Acitvate On Input
	// - Can Activate Whilst Interacting
	// - Cannot Activate Whilst Stunned
	// - Cachec Owning Character
};
