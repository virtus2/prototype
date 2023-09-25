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

	virtual void FireLeftPressed(const FInputActionValue& Value);

	virtual void FireLeftReleased(const FInputActionValue& Value);

	virtual void FireRightPressed(const FInputActionValue& Value);

	virtual void FireRightReleased(const FInputActionValue& Value);

private:


public:

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	// UPROPERTY()
	// TObjectPtr<UAnimInstance> AnimInstance;

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
	TObjectPtr<UInputAction> FireLeftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> FireRightAction;

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


private:


};
