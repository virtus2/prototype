// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "AttributeGameplayEffectMap.generated.h"

UCLASS()
class THIRDPERSON_API UAttributeGameplayEffectMap : public UDataAsset
{
	GENERATED_BODY()
   /* 
	* 특정 Attribute에 SetByCaller해주는 GameplayEffect들에 대한 맵
	* 
	* Unreal Slacker 디스코드에 의하면 GameplayEffect를 런타임에 생성해 사용하는건 비추천이라고 한다...
	* 하나의 큰 GameplayEffect를 생성해서 몇 십개의 Attribute를 설정해주는 것도 지연시간이 크다고 한다...
	* 따라서 Attribute마다 하나씩 GameplayEffect를 만들고, 여기에 캐싱해놓고 사용한다.
	*/
public:
	TSubclassOf<UGameplayEffect> GetGameplayEffectByAttributeTag(FGameplayTag AttributeTag);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> AttributeToGameplayEffect;
	
};
