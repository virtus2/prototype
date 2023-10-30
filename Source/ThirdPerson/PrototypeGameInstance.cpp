// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeGameInstance.h"

#include "GameplayTagContainer.h"

#include "ThirdPerson/Game/Item/PrototypeEquipmentType.h"

UPrototypeGameInstance::UPrototypeGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentTypeDataTableObject(TEXT("/Game/Prototype/Data/DT_EquipmentType"));
	if (EquipmentTypeDataTableObject.Succeeded())
	{
		EquipmentTypeDataTable = EquipmentTypeDataTableObject.Object;
		UE_LOG(LogTemp, Warning, TEXT("EquipmentTypeDataTable Loaded Successfully"));
	}
}

void UPrototypeGameInstance::Init()
{
    Super::Init(); 
}

