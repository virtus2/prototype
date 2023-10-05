// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PrototypePlayerState.generated.h"

class APrototypePlayerController;
class UAbilitySystemComponent;
class UPrototypeAbilitySystemComponent;

UCLASS()
class THIRDPERSON_API APrototypePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APrototypePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Prototype|PlayerState")
	APrototypePlayerController* GetPrototypePlayerController() const;
	
	UPrototypeAbilitySystemComponent* GetPrototypeAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

public:

protected:

private:
	/* 
	액터가 소멸되어 리스폰될 수 있는 경우에는 어빌리티 시스템 컴포넌트를 플레이어 스테이트에 보관하는 것이 좋다. 
	다수의 액터가 하나의 어빌리티 시스템 컴포넌트를 공유할 수 있다.
	*/
	UPROPERTY(VisibleAnywhere, Category = "Prototype|PlayerState")
	TObjectPtr<UPrototypeAbilitySystemComponent> AbilitySystemComponent;
};
