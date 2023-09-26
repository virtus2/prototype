// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FTimerHandle;

UCLASS()
class THIRDPERSON_API APrototypeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APrototypeCharacter();

protected:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;

    virtual void Move(const FInputActionValue& Value);

    virtual void Look(const FInputActionValue& Value);

    virtual void JumpStart(const FInputActionValue& Value);

    virtual void JumpStop(const FInputActionValue& Value);

    virtual void BasicFireStart(const FInputActionValue& Value);

    virtual void BasicFireStop(const FInputActionValue& Value);

    virtual void SkillUseStart(const FInputActionValue& Value);

    virtual void SkillUseStop(const FInputActionValue& Value);

    virtual void TryBasicFire();

    virtual void BasicFire();

    virtual void BasicFireTimerFinished();

private:


public:

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    /*
        Enhanced Input...
    */
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> BasicFireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SkillAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveRAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveEAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveQAction;

    /*
        Character Status... 
    */
    UPROPERTY(EditDefaultsOnly, Category ="Character Status")
    float CurrentHealthPoint;

    UPROPERTY(EditDefaultsOnly, Category = "Character Status")
    float MaxHealthPoint;

    UPROPERTY(VisibleAnywhere, Category="Character Status|Combat")
    bool bBasicFireStarted;

    UPROPERTY(VisibleAnywhere, Category = "Character Status|Combat")
    bool bBasicFireReady = true;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Status|Combat")
    float BasicFireDelay;

    FTimerHandle BasicFireTimerHandle;


private:


};
