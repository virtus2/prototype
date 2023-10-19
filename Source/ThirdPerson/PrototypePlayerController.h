// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrototypePlayerController.generated.h"

class UPrototypeHUDWidget;

UCLASS()
class THIRDPERSON_API APrototypePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void CreateHUD();
	
protected:
	// Server Only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

private:


public:
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Prototype|UI")
	TSubclassOf<UPrototypeHUDWidget> PrototypeHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category="Prototype|UI")
	TObjectPtr<UPrototypeHUDWidget> PrototypeHUDWidget;

};
