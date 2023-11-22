#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ItemType.generated.h"

USTRUCT(BlueprintType)
struct FItemType : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, meta=(Categories = "Item.Type"))
    FGameplayTag ItemTypeTag;

    UPROPERTY(EditAnywhere)
    bool bCanEquip;

    UPROPERTY(EditAnywhere, meta=(EditCondition="bCanEquip", Cateogires = "Character.EquipSlot"))
    FGameplayTag EquipSlotTag;

    UPROPERTY(EditAnywhere)
    bool bCanHaveSockets;

    // TODO: 그냥 다른 데이터 테이블에서 정해주는 게 나을수도
    // 예를들면 Helmet타입은 15레벨 2개, 30레벨 3개, 45레벨 4개
    UPROPERTY(EditAnywhere, meta=(EditCondition="bCanHaveSockets"))
    TMap<int32, int8> MaxSocketCount; // <Item Level, Max Socket Count>
    
    UPROPERTY(EditAnywhere)
    int TreasureClassPickWeight;

    UPROPERTY(EditAnywhere)
    int InventoryWidth; 

    UPROPERTY(EditAnywhere)
    int InventoryHeight;
};
