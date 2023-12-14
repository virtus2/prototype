#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ItemBase.generated.h"

USTRUCT()
struct FItemBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FString InGameName;

    UPROPERTY(EditAnywhere, meta = (Categories = "Item.Type"))
    FGameplayTag ItemType;

    UPROPERTY(EditAnywhere)
    int32 Level = 1;

    UPROPERTY(EditAnywhere)
    int32 LevelRequirement = 0;

    UPROPERTY(EditAnywhere)
    int32 Rarity = 1;

    UPROPERTY(EditAnywhere)
    bool bSpawnable = true;
};
