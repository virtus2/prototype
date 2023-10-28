// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PrototypeItemModifierType.generated.h"

USTRUCT(BlueprintType)
struct THIRDPERSON_API FPrototypeItemModifierType : public FTableRowBase
{
	GENERATED_BODY()
	 
public:
	/* 아이템 모디파이어 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ModifierTypeName;

	/* 모디파이어 그룹 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ModifierGroupName;

	/* 효과 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int> ModifierEffect;
	
	/* 인게임 표시 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DisplayName;

	/* 모디파이어 동일 그룹 내 가중치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ModifierWeight;

	/* 모디파이어가 붙을 수 있는 아이템 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> ModifierAppearOnItemType;

	/* 모디파이어가 붙을 수 있는 최소 아이템 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinItemLevel;

	/* 모디파이어가 붙을 수 있는 최대 아이템 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxItemLevel;
};
