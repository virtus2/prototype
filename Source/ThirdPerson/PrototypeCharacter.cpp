// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"	
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

APrototypeCharacter::APrototypeCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APrototypeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APrototypeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APrototypeCharacter::JumpStart);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APrototypeCharacter::JumpStop);
	EnhancedInputComponent->BindAction(FireLeftAction, ETriggerEvent::Started, this, &APrototypeCharacter::FireLeftPressed);
	EnhancedInputComponent->BindAction(FireLeftAction, ETriggerEvent::Completed, this, &APrototypeCharacter::FireLeftReleased);
	EnhancedInputComponent->BindAction(FireRightAction, ETriggerEvent::Started, this, &APrototypeCharacter::FireRightPressed);
	EnhancedInputComponent->BindAction(FireRightAction, ETriggerEvent::Completed, this, &APrototypeCharacter::FireRightReleased);
}

void APrototypeCharacter::Move(const FInputActionValue& Value)
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

void APrototypeCharacter::Look(const FInputActionValue& Value)
{
	auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
	AddControllerYawInput(AxisValues.X);
	AddControllerPitchInput(AxisValues.Y);
}

void APrototypeCharacter::JumpStart(const FInputActionValue& Value)
{
	Jump();
}

void APrototypeCharacter::JumpStop(const FInputActionValue& Value)
{
	StopJumping();
}

void APrototypeCharacter::FireLeftPressed(const FInputActionValue& Value)
{

}

void APrototypeCharacter::FireLeftReleased(const FInputActionValue& Value)
{
}

void APrototypeCharacter::FireRightPressed(const FInputActionValue& Value)
{
}

void APrototypeCharacter::FireRightReleased(const FInputActionValue& Value)
{
}


