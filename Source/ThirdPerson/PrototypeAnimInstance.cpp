// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UPrototypeAnimInstance::UPrototypeAnimInstance()
{
}

void UPrototypeAnimInstance::NativeInitializeAnimation()
{
    PawnOwner = TryGetPawnOwner();
    if (IsValid(PawnOwner))
    {
        Character = Cast<ACharacter>(PawnOwner);
        if (IsValid(Character))
        {
            CharacterMovement = Character->GetCharacterMovement();
        }
    }
}

void UPrototypeAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    if (!IsValid(PawnOwner))
    {
        return;
    }
    if (!IsValid(Character))
    {
        return;
    }
    if (!IsValid(CharacterMovement))
    {
        return;
    }

    // Set IsInAir
    bIsInAir = CharacterMovement->IsFalling();

    // Set Speed
    FVector Velocity = PawnOwner->GetVelocity();
    Speed = Velocity.Length();

    // Set Roll, Pitch and Yaw
    FRotator BaseAimRotation = PawnOwner->GetBaseAimRotation();
    FRotator ActorRotation = PawnOwner->GetActorRotation();
    FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(BaseAimRotation, ActorRotation);
    Roll = DeltaRotation.Roll;
    Pitch = DeltaRotation.Pitch;
    YawLastTick = Yaw;
    Yaw = ActorRotation.Yaw;
    YawChangeOverFrame = UKismetMathLibrary::NormalizeAxis(YawLastTick - Yaw);

    // Set YawDelta for Leans
    float YawDeltaInterpTarget = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, ActorRotation).Yaw / DeltaSeconds / LeanIntensity;
    YawDelta = UKismetMathLibrary::FInterpTo(YawDelta, YawDeltaInterpTarget, DeltaSeconds, LeanInterpSpeed);
    RotationLastTick = ActorRotation;

    // Set IsAccelerating
    bIsAccelerating = CharacterMovement->GetCurrentAcceleration().Length() > 0.0f;

    // Set Direction and StopDirection
    float DirectionInterpSpeed = UKismetMathLibrary::Abs(StopDirection) < 170.0f ? 15.0f : 0.0f;
    float TargetDirection = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), BaseAimRotation).Yaw;
    Direction = UKismetMathLibrary::FInterpTo(StopDirection, TargetDirection, DeltaSeconds, DirectionInterpSpeed);
    if (bIsAccelerating)
    {
        StopDirection = Direction;
    }

    // Is Any Montage Playing
    //
    //
    
    // Is Fullbody
    //
    //



    if (bIsAccelerating || Speed > 0.0f)
    {
        // If Character is moving
        RootYawOffset = UKismetMathLibrary::FInterpTo(RootYawOffset, 0.0f, DeltaSeconds, 15.0f);
        RootYawOffset = FMath::Clamp(RootYawOffset, -180, 180);
    }
    else
    {
        RootYawOffset += YawChangeOverFrame;
        RootYawOffset = FMath::Clamp(RootYawOffset, -180, 180);
        
        float TurnAnimCurveValue;
        GetCurveValue(TEXT("TurnAnim"), TurnAnimCurveValue);
        bIsTurnAnimPlaying = FMath::IsNearlyEqual(TurnAnimCurveValue, 1.0f, 0.001f);
        if (bIsTurnAnimPlaying)
        {
            if (bIsFirstFrameOfTurnAnim)
            {
                // On the first frame of a turn anim, grab the curve value before setting 'Curve Value Last Frame' so that we don't get a pop.
                GetCurveValue(TEXT("DistanceCurve"), CurveValue);
                bIsFirstFrameOfTurnAnim = false;
            }
            CurveValueLastFrame = CurveValue;
            GetCurveValue(TEXT("DistanceCurve"), CurveValue);

            float TurnDirection = RootYawOffset > 0.0f ? -1.0f : 1.0f;
            float TurnedAmount = TurnDirection * (CurveValueLastFrame - CurveValue);
            RootYawOffset -= TurnedAmount;
        }
        else
        {
            GetCurveValue(TEXT("DistanceCurve"), CurveValue);
            bIsFirstFrameOfTurnAnim = true;
        }
    }
}

void UPrototypeAnimInstance::AnimNotify_SaveAttack()
{
}

void UPrototypeAnimInstance::AnimNotify_ResetCombo()
{
}
