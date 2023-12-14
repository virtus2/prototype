#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemBase.h"

#include "Armor.generated.h"

USTRUCT(BlueprintType)
struct FArmor : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere)
    FItemBase BaseData;
    
    /* 최소 방어도 */
    UPROPERTY(EditAnywhere)
    int32 MinDef;

    /* 최대 방어도 */
    UPROPERTY(EditAnywhere)
    int32 MaxDef;
};
