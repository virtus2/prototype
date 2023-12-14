#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ItemBase.h"

#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeapon : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere)
    FItemBase BaseData;
};
