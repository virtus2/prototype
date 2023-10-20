// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PrototypeAnimInstance.generated.h"

class APawn;
class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class THIRDPERSON_API UPrototypeAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
    
public:
    UPrototypeAnimInstance();

protected:
    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UFUNCTION()
    void AnimNotify_SaveAttack();

    UFUNCTION()
    void AnimNotify_ResetCombo();

private:

public:

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="States")
    bool bIsInAir;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsAccelerating;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsAttacking;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsTurning;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsHit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsFirstFrameOfTurnAnim;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
    bool bIsTurnAnimPlaying;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float Pitch;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float Roll;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float Yaw;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float YawDelta;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float RootYawOffset;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float ActorIdleStartRotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float YawLastTick;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float YawChangeOverFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float CurveValue;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float CurveValueLastFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
    float StopDirection;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRotator RotationLastTick;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ACharacter> Character;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<APawn> PawnOwner;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UCharacterMovementComponent> CharacterMovement;


private:
    /* lower number is more intense */
    UPROPERTY(EditAnywhere, Meta=(AllowPrivateAccess=true))
    float LeanIntensity = 5.0f;

    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
    float LeanInterpSpeed = 6.0f;

};
