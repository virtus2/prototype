#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FString InGameName;

    UPROPERTY(EditAnywhere, meta = (Categories = "Item.Type"))
    FGameplayTag ItemType;

    UPROPERTY(EditAnywhere)
    int32 Level;

    UPROPERTY(EditAnywhere)
    int32 LevelRequirement;
};
