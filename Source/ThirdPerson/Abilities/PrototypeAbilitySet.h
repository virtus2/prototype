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

USTRUCT(BlueprintType)
struct FPrototypeAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UPrototypeAbilitySystemComponent* PrototypeASC);

protected:
	// Handles to the granted abilities.
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/*
캐릭터에게 부여할 GameplayAbility와 GameplayEffect
 */
UCLASS()
class THIRDPERSON_API UPrototypeAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPrototypeAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UPrototypeAbilitySystemComponent* PrototypeASC, FPrototypeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<TSubclassOf<UPrototypeGameplayAbility>> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<TSubclassOf<UGameplayEffect>> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
	TArray<TSubclassOf<UAttributeSet>> GrantedAttributes;
};
