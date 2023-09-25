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
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float CurrentHealthPoint, float MaxHealthPoint);

protected:

private:


public:


protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthPointTextBlock;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthPointProgressBar;

private:

	
};
