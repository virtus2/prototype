// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PrototypeEquipmentType.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FPrototypeEquipmentType: public FTableRowBase
{
	GENERATED_BODY()

	/*
	 * 아이템 장비 타입에 대해서 정의
	 */

public:
	/* 아이템 타입 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTypeName;

	/* 아이템이 드랍될 수 있는 최소 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipmentSlotTypeName;

	/* 홈을 가질 수 있는지에 대한 flag */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanHaveSockets;

	/* 홈 최대 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxSocketCount;

	/* 홈 최소 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinSocketCount;

	/* 인벤토리 내에서 아이템의 너비 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventoryItemWidth;

	/* 인벤토리 내에서 아이템의 높이 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InventoryItemHeight;
};