// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeHeroCharacter.h"

#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThirdPerson/Player/PrototypePlayerState.h"
#include "ThirdPerson/Player/PrototypePlayerController.h"
#include "ThirdPerson/Character/PrototypeInventoryComponent.h"
#include "ThirdPerson/Abilities/PrototypeAttributeSet.h"
#include "ThirdPerson/Abilities/PrototypeAbilitySystemComponent.h"
#include "ThirdPerson/Input/PrototypeEnhancedInputComponent.h"

APrototypeHeroCharacter::APrototypeHeroCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetCapsuleComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0, 0, 68.492264));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    FollowCamera->SetupAttachment(SpringArmComponent);
    FollowCamera->FieldOfView = 80.0f;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    Inventory = CreateDefaultSubobject<UPrototypeInventoryComponent>(TEXT("Inventory"));
}

void APrototypeHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

    auto EnhancedInputComponent = Cast<UPrototypeEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }

    // EnhancedInput Bind Actions
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrototypeHeroCharacter::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrototypeHeroCharacter::Look);
    EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &APrototypeHeroCharacter::ActivateInventory);
    // Gameplay Ability System Bind Input
    EnhancedInputComponent->BindAbilityActions(InputConfig, this, &APrototypeHeroCharacter::AbilityInputTagPressed, &APrototypeHeroCharacter::AbilityInputTagReleased, &APrototypeHeroCharacter::AbilityInputTagHeld);
}

void APrototypeHeroCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitAbilityActorInfo();
    InitializeAttributes();
    AddCharacterAbilities();
    SetHealth(GetMaxHealth());
    SetMana(GetMaxMana());

    TObjectPtr<APrototypePlayerController> PC = Cast<APrototypePlayerController>(GetController());
    if (!IsValid(PC))
    {
        return;
    }
    PC->CreateHUD();
}

void APrototypeHeroCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitAbilityActorInfo();
    TObjectPtr<APrototypePlayerController> PC = Cast<APrototypePlayerController>(GetController());
    if (!IsValid(PC))
    {
        return;
    }
    PC->CreateHUD();
}

void APrototypeHeroCharacter::Move(const FInputActionValue& Value)
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

void APrototypeHeroCharacter::Look(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    AddControllerYawInput(AxisValues.X);
    AddControllerPitchInput(AxisValues.Y);
}

void APrototypeHeroCharacter::JumpStart(const FInputActionValue& Value)
{
    Jump();
}

void APrototypeHeroCharacter::JumpStop(const FInputActionValue& Value)
{
    StopJumping();
}

void APrototypeHeroCharacter::InitAbilityActorInfo()
{
    Super::InitAbilityActorInfo();

    TObjectPtr<APrototypePlayerState> PS = GetPlayerState<APrototypePlayerState>();
    if (!IsValid(PS))
    {
        return;
    }

    AbilitySystemComponent = Cast<UPrototypeAbilitySystemComponent>(PS->GetAbilitySystemComponent());
    AbilitySystemComponent->InitAbilityActorInfo(PS, this);
    AttributeSetBase = PS->GetAttributeSetBase();
}

void APrototypeHeroCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void APrototypeHeroCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void APrototypeHeroCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagHeld(InputTag);
}

void APrototypeHeroCharacter::ActivateInventory()
{
    Inventory->ActivateInventoryWidget();
}
