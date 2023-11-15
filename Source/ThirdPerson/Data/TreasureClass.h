#pragma once

#include "CoreMinimal.h"

#include "TreasureClass.generated.h"

USTRUCT(BlueprintType)
struct FTreasure
{
    GENERATED_BODY()

    /* 해당 Treasure가 선택될 선택비 값 */
    UPROPERTY(EditAnywhere)
    int32 Probability;

    /* 드랍될 아이템의 태그 또는 TreasureClass 태그 */
    UPROPERTY(EditAnywhere, meta = (Categories = "Item.Type,Item.Treasure"))
    FGameplayTag Treasure;
};

USTRUCT(BlueprintType)
struct FTreasureClass : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Group;

    UPROPERTY(EditAnywhere)
    int32 Level;

    /* 아이템 드랍 체크 반복 횟수 */
    UPROPERTY(EditAnywhere)
    int8 NumPicks;

    UPROPERTY(EditAnywhere)
    int32 FreqNoDrop;

    UPROPERTY(EditAnywhere)
    int32 FreqMagic;

    UPROPERTY(EditAnywhere)
    int32 FreqRare;

    UPROPERTY(EditAnywhere, meta=(TitleProperty="{Treasure}: {Probability}"))
    TArray<FTreasure> Treasures;
};
