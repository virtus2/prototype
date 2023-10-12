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
}

void UPrototypeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("PreAttributeChange"));
	
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UPrototypeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute"));

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 이 게임플레이 이펙트는 Health를 변경합니다. 적용하되 우선 값을 제한합니다.
		// 이 경우 Health 베이스 값은 음수가 아니어야 합니다.
		SetHealth(FMath::Max(GetHealth(), 0.0f));
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
}

void UPrototypeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
}
