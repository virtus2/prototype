// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PrototypeInventoryComponent.generated.h"

class UPrototypeItem;
class UPrototypeInventoryWidget;

UCLASS()
class THIRDPERSON_API UPrototypeInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPrototypeInventoryComponent();

	void ActivateInventoryWidget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddItemToInventory(TObjectPtr<UPrototypeItem> Item);
	void EquipItem(TObjectPtr<UPrototypeItem> Item);

private:

public:

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UPrototypeInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPrototypeInventoryWidget> InventoryWidget;

	UPROPERTY(VisibleAnywhere)
	TMap<FGameplayTag, TObjectPtr<UPrototypeItem>> EquippedItems;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UPrototypeItem>> InventoryItems;

	UPROPERTY(VisibleAnywhere)
	bool bIsActivated;

private:
		
};
