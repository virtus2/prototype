// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PrototypePlayerState.generated.h"

class AController;
class APrototypePlayerController;
class UAbilitySystemComponent;
class UPrototypeAbilitySystemComponent;
class UPrototypeAttributeSet;

UCLASS()
class THIRDPERSON_API APrototypePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APrototypePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Prototype|PlayerState")
	APrototypePlayerController* GetPrototypePlayerController() const;
	
	UPrototypeAbilitySystemComponent* GetPrototypeAbilitySystemComponent() const { return AbilitySystemComponent; }

	// IAbilitySystemInterface 
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPrototypeAttributeSet* GetAttributeSetBase() const;

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
	// ~APlayerState interface
	virtual void ClientInitialize(AController* C) override;
	virtual void OnDeactivated();
	virtual void OnReactivated();
	// ~End of APlayerState interface



protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

public:

protected:
	/**
	 * 액터가 소멸되어 리스폰될 수 있는 경우에는 어빌리티 시스템 컴포넌트를 플레이어 스테이트에 보관하는 것이 좋다.
	 * 다수의 액터가 하나의 어빌리티 시스템 컴포넌트를 공유할 수 있다.
	 */
	UPROPERTY()
	TObjectPtr<UPrototypeAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	UPrototypeAttributeSet* AttributeSetBase;

private:

};
