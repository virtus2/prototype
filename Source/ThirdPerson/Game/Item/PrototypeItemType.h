// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PrototypeItemType.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FPrototypeItemType : public FTableRowBase
{
	GENERATED_BODY()

	/*
	 * 아이템 타입에 대해서 정의
	 */
public:
	/* 아이템 타입 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTypeName;

	/* 아이템이 드랍될 수 있는 최소 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemDropLevel;

	/* 인벤토리 내에서 아이템의 너비 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventoryItemWidth;

	/* 인벤토리 내에서 아이템의 높이 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventoryItemHeight;
};