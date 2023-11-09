// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeInventoryWidget.h"

UPrototypeInventoryWidget::UPrototypeInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsActivated = false;
}

void UPrototypeInventoryWidget::ActivateWidget()
{
	if (bIsActivated)
	{
		bIsActivated = false;
		bIsFocusable = false;
		RemoveFromViewport();
	}
	else
	{
		bIsActivated = true;
		bIsFocusable = true;
		AddToViewport();
	}
}

void UPrototypeInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// TODO: 버튼 눌렀을때 다시 닫히게 해야함
	FOnInputAction InputActionDelegate;
	InputActionDelegate.BindUFunction(this, "OnInventoryActionTriggered");
	ListenForInputAction(FName(TEXT("IA_Inventory")), IE_Pressed, true, InputActionDelegate);
}

void UPrototypeInventoryWidget::OnInventoryActionTriggered()
{
	ActivateWidget();
}
