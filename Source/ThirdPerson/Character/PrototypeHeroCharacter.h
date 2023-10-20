// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrototypeCharacterBase.h"
#include "PrototypeHeroCharacter.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UPrototypeInputConfig;
/**
 * 
 */
UCLASS()
class THIRDPERSON_API APrototypeHeroCharacter : public APrototypeCharacterBase
{
	GENERATED_BODY()
	
public:
	APrototypeHeroCharacter(const class FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

    // Client only
    virtual void OnRep_PlayerState() override;

    virtual void Move(const FInputActionValue& Value);
    virtual void Look(const FInputActionValue& Value);
    virtual void JumpStart(const FInputActionValue& Value);
    virtual void JumpStop(const FInputActionValue& Value);

private:
    virtual void InitAbilityActorInfo() override;

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

public:

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Prototype|Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Prototype|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

    /*
     *   Enhanced Input...
     */
    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> BasicFireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveRAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveEAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveQAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype|Enhanced Input")
    TObjectPtr<UPrototypeInputConfig> InputConfig;
    
	
};
