// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "ThirdPerson/Ability/PrototypeGameplayAbility.h"

void UPrototypeAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UPrototypeGameplayAbility* PrototypeAbility = Cast<UPrototypeGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(PrototypeAbility->StartupInputTag);
			// AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			UE_LOG(LogTemp, Warning, TEXT("AddCharacterAbilities: %s"), *PrototypeAbility->StartupInputTag.ToString());
			GiveAbility(AbilitySpec);
		}
	}
	// bStartupAbilitiesGiven = true;
	// AbilitiesGivenDelegate.Broadcast();
}

void UPrototypeAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilityInputTagPressed: %s"), *InputTag.ToString());
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UPrototypeAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				UE_LOG(LogTemp, Warning, TEXT("AbilityInputTagHeld: TryActivateAbility %s"), *InputTag.ToString());
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UPrototypeAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			UE_LOG(LogTemp, Warning, TEXT("AbilityInputTagReleased: %s"), *InputTag.ToString());
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}
