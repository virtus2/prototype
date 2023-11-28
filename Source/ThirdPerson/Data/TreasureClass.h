#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

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
struct FRarityFreq
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Freq;

    UPROPERTY(EditAnywhere, meta=(Categories="Item.Rarity"))
    FGameplayTag Rarity;
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

    UPROPERTY(EditAnywhere, meta=(ForceInlineRow))
    TMap<FGameplayTag, int32> FreqRarities;

    UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties, TitleProperty="{Treasure}가 {Probability}의 비율로 생성"))
    TArray<FTreasure> Treasures;
};
