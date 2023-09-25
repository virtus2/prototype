// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeHUD.h"
#include <GameFramework/PlayerController.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <ThirdPerson/PrototypeHUDWidget.h>

void APrototypeHUD::BeginPlay()
{
	InitHUD();
}

void APrototypeHUD::InitHUD()
{
	PrototypeHUDWidget = CreateWidget<UPrototypeHUDWidget>(PlayerOwner, PrototypeHUDWidgetClass);
	if (PrototypeHUDWidget)
	{
		PrototypeHUDWidget->AddToViewport();
	}
}
