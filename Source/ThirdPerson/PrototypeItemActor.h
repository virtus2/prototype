// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrototypeItemActor.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UPrototypeItem;

UCLASS()
class THIRDPERSON_API APrototypeItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrototypeItemActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetItem(TObjectPtr<UPrototypeItem> ItemInstance);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

public:

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPrototypeItem> Item;

private:
};
