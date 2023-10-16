// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PTCharacterBase.h"
#include "PTHeroCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class THIRDPERSON_API APTHeroCharacter : public APTCharacterBase
{
	GENERATED_BODY()
	
public:
	APTHeroCharacter(const class FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

    virtual void Move(const FInputActionValue& Value);
    virtual void Look(const FInputActionValue& Value);
    virtual void JumpStart(const FInputActionValue& Value);
    virtual void JumpStop(const FInputActionValue& Value);

private:

public:

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Prototype|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Prototype|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

    /*
     *   Enhanced Input...
     */
    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> BasicFireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveRAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveEAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveQAction;
	
};
