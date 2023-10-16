// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTCharacterBase.generated.h"

class UGameplayEffect;
class UPrototypeGameplayAbility;
class UPrototypeAttributeSet;
class UPrototypeAbilitySystemComponent;

UCLASS()
class THIRDPERSON_API APTCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APTCharacterBase(const class FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

public:

protected:

	/*
	 * 어빌리티 시스템 테스트
	 */

	TWeakObjectPtr<UPrototypeAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UPrototypeAttributeSet> AttributeSetBase;

	 // Default attributes for a character for initializing on spawn/respawn.
	 // This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Prototype Character|Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype Character|Abilities")
	TArray<TSubclassOf<UPrototypeGameplayAbility>> CharacterAbilities;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Prototype Character|Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

private:
};
