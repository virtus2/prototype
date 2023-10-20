// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UPrototypeAttributeSet::UPrototypeAttributeSet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPrototypeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UPrototypeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// TODO: 체력을 최대체력으로 맞춰주는거는 필요없을수도 있음
	if (Attribute == GetMaxHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PreAttributeChange MaxHealth - NewValue: %f"), NewValue);
		// AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PreAttributeChange Health - NewValue: %f"), NewValue);
	}
}

void UPrototypeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute MaxHealth"));

	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Health"));
		SetHealth(FMath::Max(GetHealth(), 0.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute MaxMana %f"),
			Data.EvaluatedData.Magnitude);
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Mana %f"),
			Data.EvaluatedData.Magnitude);
		SetHealth(FMath::Max(GetMana(), 0.0f));
	}
}

void UPrototypeAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UPrototypeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrototypeAttributeSet, Health, OldValue);
}

void UPrototypeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrototypeAttributeSet, MaxHealth, OldValue);
}

void UPrototypeAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrototypeAttributeSet, Mana, OldValue);
}

void UPrototypeAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPrototypeAttributeSet, MaxMana, OldValue);
}
