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
class FDelegateHandle;
struct FOnAttributeChangeData;

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
	
	UPrototypeAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|Attributes")
	float GetMaxMana() const;


protected:
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

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
	UPrototypeAttributeSet* AttributeSet;

private:

};
