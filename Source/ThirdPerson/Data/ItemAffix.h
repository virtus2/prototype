#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ItemAffix.generated.h"

USTRUCT(BlueprintType)
struct FAttributeModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    float MinValue;

    UPROPERTY(EditAnywhere)
    float MaxValue;

    UPROPERTY(EditAnywhere, meta = (Categories = "Ability.Attribute"))
    FGameplayTag Attribute;
};

USTRUCT(BlueprintType)
struct FItemAffix : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere)
    FString InGameName;

    UPROPERTY(EditAnywhere)
    bool bOnlySpawnRare;

    UPROPERTY(EditAnywhere)
    int32 Level;

    UPROPERTY(EditAnywhere)
    int32 MaxLevel = 999;

    UPROPERTY(EditAnywhere)
    int32 LevelRequirement;

    UPROPERTY(EditAnywhere)
    int32 Frequency;

    UPROPERTY(EditAnywhere, meta = (Categories = "Item.Affix.Type"))
    FGameplayTag AffixType;
 
    UPROPERTY(EditAnywhere, meta = (Categories = "Item.Affix.Group"))
    FGameplayTag AffixGroup;

    UPROPERTY(EditAnywhere)
    TArray<FAttributeModifier> AffixModifiers;

    UPROPERTY(EditAnywhere, meta=(Categories="Item.Type"))
    FGameplayTagContainer SpawnableItemTypes;
};
