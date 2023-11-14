#pragma once

#include "CoreMinimal.h"

#include "TreasureClass.generated.h"

USTRUCT(BlueprintType)
struct FTreasure
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Probability;

    UPROPERTY(EditAnywhere)
    FGameplayTag Treasure;
};

USTRUCT(BlueprintType)
struct FTreasureClass
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Group;

    UPROPERTY(EditAnywhere)
    int32 Level;

    UPROPERTY(EditAnywhere)
    TArray<FTreasure> Treasures;
};
