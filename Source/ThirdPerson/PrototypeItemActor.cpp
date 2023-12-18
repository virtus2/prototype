// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItemActor.h"

#include "Components/BoxComponent.h"

#include "ThirdPerson/Item/PrototypeItem.h"
#include "ThirdPerson/Character/PrototypeHeroCharacter.h"

// Sets default values
APrototypeItemActor::APrototypeItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void APrototypeItemActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) 
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APrototypeItemActor::OnOverlapBegin);
	}
	
}

void APrototypeItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto HeroCharacter = Cast<APrototypeHeroCharacter>(OtherActor);
	if (IsValid(HeroCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hero Overlapped"));
	}
}

// Called every frame
void APrototypeItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APrototypeItemActor::SetItem(TObjectPtr<UPrototypeItem> ItemInstance)
{
	this->Item = ItemInstance;
	SkeletalMeshComponent->SetSkeletalMesh(Item->Mesh);
}

