// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAttributeSet.h"

#include "Net/UnrealNetwork.h"

UPrototypeAttributeSet::UPrototypeAttributeSet()
{
}

void UPrototypeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPrototypeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPrototypeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
}

void UPrototypeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
}
