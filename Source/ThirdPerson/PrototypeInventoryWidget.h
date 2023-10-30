	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PrototypeInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_API UPrototypeInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPrototypeInventoryWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE bool IsActivated() { return bIsActivated; }

	void ActivateWidget();

protected:
	virtual void NativeConstruct() override;
	virtual void OnInventoryActionTriggered();

private:

public:

protected:
	UPROPERTY(VisibleAnywhere)
	bool bIsActivated;

private:

	
};
