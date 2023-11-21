// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PrototypeCharacterBase.h"

#include "GameplayTagContainer.h"

#include "PrototypeEnemyCharacter.generated.h"

struct FGameplayTag;
class UBehaviorTree;
class APrototypeAIController;
class UPrototypeAnimInstance;

UCLASS()
class THIRDPERSON_API APrototypeEnemyCharacter : public APrototypeCharacterBase
{
	GENERATED_BODY()
	
public:
	APrototypeEnemyCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void SpawnTreasure();

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
	TObjectPtr<APrototypeAIController> PrototypeAIController;

	UPROPERTY(EditAnywhere, Category="Prototype")
	int32 MonsterLevel;

	/* Enemy 데이터 */
	UPROPERTY(EditDefaultsOnly, Category="Prototype|Data")
	TObjectPtr<USkeletalMesh> EnemyMesh;

	UPROPERTY(EditDefaultsOnly, Category="Prototype|Data")
	TObjectPtr<UPrototypeAnimInstance> EnemyAnimInstance;

	UPROPERTY(EditDefaultsOnly, Category="Prototype|Data", meta=(Categories="Item.Treasure"))
	FGameplayTag TreasureClass;

	UPROPERTY(EditDefaultsOnly, Category="Prototype|Data")
	float Level;

	/* Enemy 데이터 끝*/
};
