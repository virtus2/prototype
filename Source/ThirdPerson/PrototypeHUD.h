// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PrototypeHUD.generated.h"

class UUserWidget;
class UPrototypeHUDWidget;

UCLASS()
class THIRDPERSON_API APrototypeHUD : public AHUD
{
	GENERATED_BODY()

public:

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitHUD();

private:


public:

protected:
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UPrototypeHUDWidget> PrototypeHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UPrototypeHUDWidget> PrototypeHUDWidget;

private:

};
