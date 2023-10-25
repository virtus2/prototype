// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeGameItemActor.h"

#include "Components/SphereComponent.h"

#include "ThirdPerson/Abilities/PrototypeAbilitySet.h"

// Sets default values
APrototypeGameItemActor::APrototypeGameItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void APrototypeGameItemActor::BeginPlay()
{
	Super::BeginPlay();

	

	for (auto& Affix : GameItemData.GameItemAffix)
	{
		for (auto& Attribute : Affix.AttributeMinMax)
		{
			Attribute.Value.ActualValue = FMath::RandRange(Attribute.Value.MinValue, Attribute.Value.MaxValue);
		}
	}
}

// Called every frame
void APrototypeGameItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

