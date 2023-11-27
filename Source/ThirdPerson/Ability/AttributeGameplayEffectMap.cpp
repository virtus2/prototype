// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeGameplayEffectMap.h"

TSubclassOf<UGameplayEffect> UAttributeGameplayEffectMap::GetGameplayEffectByAttributeTag(FGameplayTag AttributeTag)
{
	auto GE = AttributeToGameplayEffect.Find(AttributeTag);
	if (GE)
	{
		return GE->Get();
	}
	else
	{
		return nullptr;
	}
}
