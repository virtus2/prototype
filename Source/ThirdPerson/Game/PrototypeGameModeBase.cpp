// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeGameModeBase.h"

#include "ThirdPerson/Item/PrototypeItemGenerator.h"

APrototypeGameModeBase::APrototypeGameModeBase()
{
}

void APrototypeGameModeBase::BeginPlay()
{
	ItemGenerator = NewObject<UPrototypeItemGenerator>(GetWorld());
}



