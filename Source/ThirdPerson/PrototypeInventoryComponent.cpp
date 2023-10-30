// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeInventoryComponent.h"

#include "ThirdPerson/PrototypeItem.h"
#include "ThirdPerson/PrototypeInventoryWidget.h"
#include "ThirdPerson/Character/PrototypeHeroCharacter.h"

// Sets default values for this component's properties
UPrototypeInventoryComponent::UPrototypeInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bIsActivated = false;
	// TODO: 인벤토리 크기도 데이터 테이블의 값에서 읽어올까??
}

void UPrototypeInventoryComponent::ActivateInventoryWidget()
{
	APrototypeHeroCharacter* HeroCharacter = Cast<APrototypeHeroCharacter>(GetOwner());
	if (!IsValid(HeroCharacter))
	{
		return;
	}

	APlayerController* PlayerController = HeroCharacter->GetLocalViewingPlayerController();
	if (!IsValid(PlayerController))
	{
		return;
	}

	if (!IsValid(InventoryWidget))
	{
		InventoryWidget = CreateWidget<UPrototypeInventoryWidget>(PlayerController, InventoryWidgetClass);
		if (!IsValid(InventoryWidget))
		{
			return;
		}
	}

	if (bIsActivated)
	{
		const FInputModeGameOnly InputModeOptions;
		PlayerController->SetInputMode(InputModeOptions);
		PlayerController->bShowMouseCursor = false;
	}
	else
	{
		const FInputModeUIOnly InputModeOptions;
		PlayerController->SetInputMode(InputModeOptions);
		PlayerController->bShowMouseCursor = true;
	}
	InventoryWidget->ActivateWidget();
}

void UPrototypeInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPrototypeInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPrototypeInventoryComponent::AddItemToInventory(TObjectPtr<UPrototypeItem> Item)
{
	InventoryItems.Add(Item);
}

void UPrototypeInventoryComponent::EquipItem(TObjectPtr<UPrototypeItem> Item)
{
	if (EquippedItems.Contains(Item->EquipmentType))
	{
		EquippedItems.Remove(Item->EquipmentType);
	}
	else
	{
		EquippedItems.Add(Item->EquipmentType, Item);
	}
}

