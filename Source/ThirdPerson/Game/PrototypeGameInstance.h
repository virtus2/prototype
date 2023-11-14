// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "PrototypeGameInstance.generated.h"

struct FPrototypeEquipmentType;

UCLASS()
class THIRDPERSON_API UPrototypeGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPrototypeGameInstance();

protected:
	virtual void Init() override;

private:

public:

protected:

private:

};
