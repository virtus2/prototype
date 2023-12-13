#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item.h"

#include "Armor.generated.h"

struct FItem;

USTRUCT(BlueprintType)
struct FArmor : public FItem
{
    GENERATED_BODY()
    
    /* 최소 방어도 */
    UPROPERTY(EditAnywhere)
    int32 MinDef;

    /* 최대 방어도 */
    UPROPERTY(EditAnywhere)
    int32 MaxDef;
};
