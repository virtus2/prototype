// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrototypeGameModeBase.generated.h"

class UDataTable;

UCLASS()
class THIRDPERSON_API APrototypeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Prototype")
	TObjectPtr<UDataTable> TreasureClassDataTable;
};
