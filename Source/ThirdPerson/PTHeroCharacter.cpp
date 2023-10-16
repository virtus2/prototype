// Fill out your copyright notice in the Description page of Project Settings.


#include "PTHeroCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "PrototypeAbilitySystemComponent.h"

APTHeroCharacter::APTHeroCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetCapsuleComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0, 0, 68.492264));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    FollowCamera->SetupAttachment(SpringArmComponent);
    FollowCamera->FieldOfView = 80.0f;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void APTHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    auto PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        return;
    }

    auto EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!EnhancedInputSubsystem)
    {
        return;
    }
    
    EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

    auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }

    // EnhancedInput Bind Actions
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APTHeroCharacter::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APTHeroCharacter::Look);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APTHeroCharacter::JumpStart);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APTHeroCharacter::JumpStop);

    // Gameplay Ability System Bind Input
    if (AbilitySystemComponent.IsValid())
    {
        FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/PrototypeEnum"), FName("EPrototypeAbilityInputID"));
        /*
        AbilitySystemComponent->BindAbilityActivationToInputComponent(EnhancedInputComponent, 
            FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), 
        */
    }
    
}

void APTHeroCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

}

void APTHeroCharacter::Move(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    FRotator CharacterRotation = GetControlRotation();

    // Left / Right
    FRotator RightRotation = FRotator(0.0f, CharacterRotation.Yaw, CharacterRotation.Roll);
    FVector RightDirection = UKismetMathLibrary::GetRightVector(RightRotation);
    AddMovementInput(RightDirection, AxisValues.X);

    // Forward / Backward
    FRotator ForwardRotation = FRotator(0.0f, CharacterRotation.Yaw, 0.0f);
    FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(ForwardRotation);
    AddMovementInput(ForwardDirection, AxisValues.Y);
}

void APTHeroCharacter::Look(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    AddControllerYawInput(AxisValues.X);
    AddControllerPitchInput(AxisValues.Y);
}

void APTHeroCharacter::JumpStart(const FInputActionValue& Value)
{
    Jump();
}

void APTHeroCharacter::JumpStop(const FInputActionValue& Value)
{
    StopJumping();
}
