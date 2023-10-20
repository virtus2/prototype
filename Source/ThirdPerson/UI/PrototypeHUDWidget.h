// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PrototypeHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class THIRDPERSON_API UPrototypeHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetMaxHealth(float InMaxHealth);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCurrentHealth(float InCurrentHealth);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float InHealthPercentage);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetMaxMana(float InMaxMana);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCurrentMana(float InCurrentMana);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetManaPercentage(float InManaPercentage);

protected:

private:

public:

protected:
    UPROPERTY(BlueprintReadWrite)
    float MaxHealth;

    UPROPERTY(BlueprintReadWrite)
    float CurrentHealth;

    UPROPERTY(BlueprintReadWrite)
    float HealthPercentage;

    UPROPERTY(BlueprintReadWrite)
    float MaxMana;

    UPROPERTY(BlueprintReadWrite)
    float CurrentMana;

    UPROPERTY(BlueprintReadWrite)
    float ManaPercentage;

};
