// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "Engine/DataAsset.h"

#include "PrototypeAbilitySet.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UPrototypeAbilitySystemComponent;
class UPrototypeGameplayAbility;
class UObject;

UCLASS()
class THIRDPERSON_API UPrototypeAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPrototypeAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UPrototypeAbilitySystemComponent* PrototypeASC);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<TSubclassOf<UPrototypeGameplayAbility>> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<TSubclassOf<UGameplayEffect>> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
	TArray<TSubclassOf<UAttributeSet>> GrantedAttributes;
};
