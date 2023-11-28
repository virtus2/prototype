// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypePlayerController.h"

#include "ThirdPerson/UI/PrototypeHUDWidget.h"
#include "ThirdPerson/Player/PrototypePlayerState.h"
#include "ThirdPerson/Ability/PrototypeAbilitySystemComponent.h"

void APrototypePlayerController::CreateHUD()
{
	APrototypePlayerState* PS = GetPlayerState<APrototypePlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	// 로컬 플레이어 컨트롤러일때만 HUD를 생성한다.
	if (!IsLocalPlayerController())
	{
		return;
	}

	PrototypeHUDWidget = CreateWidget<UPrototypeHUDWidget>(this, PrototypeHUDWidgetClass);
	if (!IsValid(PrototypeHUDWidget))
	{
		return;
	}

	PrototypeHUDWidget->AddToViewport();
	// TODO: HUD에 Attribute 값 세팅
	PrototypeHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	PrototypeHUDWidget->SetCurrentHealth(PS->GetHealth());
	PrototypeHUDWidget->SetMaxMana(PS->GetMaxMana());
	PrototypeHUDWidget->SetCurrentMana(PS->GetMana());
}

TObjectPtr<UPrototypeHUDWidget> APrototypePlayerController::GetHUD()
{
	return PrototypeHUDWidget;
}

void APrototypePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TObjectPtr<APrototypePlayerState> PS = GetPlayerState<APrototypePlayerState>();
	if (!IsValid(PS))
	{
		return;
	}

	TObjectPtr<UAbilitySystemComponent> ASC = PS->GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}
	ASC->InitAbilityActorInfo(PS, InPawn);
}

void APrototypePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}