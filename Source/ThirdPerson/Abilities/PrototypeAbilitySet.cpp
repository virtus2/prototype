// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAbilitySet.h"

#include "PrototypeAbilitySystemComponent.h"
#include "PrototypeGameplayAbility.h"

void FPrototypeAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FPrototypeAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FPrototypeAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FPrototypeAbilitySet_GrantedHandles::TakeFromAbilitySystem(UPrototypeAbilitySystemComponent* PrototypeASC)
{
	if (!IsValid(PrototypeASC))
	{
		return;
	}

	if (!PrototypeASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			PrototypeASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			PrototypeASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		PrototypeASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

UPrototypeAbilitySet::UPrototypeAbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPrototypeAbilitySet::GiveToAbilitySystem(UPrototypeAbilitySystemComponent* PrototypeASC, FPrototypeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	if (!IsValid(PrototypeASC))
	{
		return;
	}

	if (!PrototypeASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const auto AbilityCDO = GrantedGameplayAbilities[AbilityIndex].GetDefaultObject();

		if (!IsValid(AbilityCDO))
		{
			UE_LOG(LogTemp, Warning, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, 0);
		AbilitySpec.SourceObject = SourceObject;

		const FGameplayAbilitySpecHandle AbilitySpecHandle = PrototypeASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const auto GameplayEffect = GrantedGameplayEffects[EffectIndex].GetDefaultObject();

		if (!IsValid(GameplayEffect))
		{
			UE_LOG(LogTemp, Warning, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const FActiveGameplayEffectHandle GameplayEffectHandle = PrototypeASC->ApplyGameplayEffectToSelf(GameplayEffect, 0, PrototypeASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		UAttributeSet* NewSet = NewObject<UAttributeSet>(PrototypeASC->GetOwner(), GrantedAttributes[SetIndex]);
		if (!IsValid(NewSet))
		{
			UE_LOG(LogTemp, Warning, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}
		PrototypeASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}

