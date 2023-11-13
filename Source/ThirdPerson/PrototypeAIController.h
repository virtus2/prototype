// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PrototypeAIController.generated.h"

class FName;
class UBehaviorTree;
class UBlackboardData;

UCLASS()
class THIRDPERSON_API APrototypeAIController : public AAIController
{
	GENERATED_BODY()

public:
	APrototypeAIController();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

protected:

private:
	void OnRepeatTimer();

public:
	const static FName HomePosKey;

	const static FName PatrolPosKey;

	const static FName TargetKey;

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY()
	TObjectPtr<UBlackboardData> BlackBoardAsset;


private:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
