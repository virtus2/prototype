// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PrototypeCharacterBase.h"
#include "PrototypeEnemyCharacter.generated.h"

class UBehaviorTree;
class APrototypeAIController;

UCLASS()
class THIRDPERSON_API APrototypeEnemyCharacter : public APrototypeCharacterBase
{
	GENERATED_BODY()
	
public:
	APrototypeEnemyCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() override;


private:

public:

protected:
	UPROPERTY(EditAnywhere, Category="Prototype|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY(EditAnywhere, Category="Prototype|AI")
	TObjectPtr<APrototypeAIController>	PrototypeAIController;


};
