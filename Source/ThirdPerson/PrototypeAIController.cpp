// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName APrototypeAIController::HomePosKey(TEXT("HomePos"));
const FName APrototypeAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName APrototypeAIController::TargetKey(TEXT("Target"));

APrototypeAIController::APrototypeAIController()
{
    RepeatInterval = 3.0f;
    static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackBoardClass(TEXT("/Game/ThirdPerson/BB_PrototypeCharacter.BB_PrototypeCharacter"));
    if (BlackBoardClass.Succeeded())
    {
        BlackBoardAsset = BlackBoardClass.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeClass(TEXT("/Game/ThirdPerson/BT_PrototypeCharacter.BT_PrototypeCharacter"));
    if (BehaviorTreeClass.Succeeded())
    {
        BehaviorTreeAsset = BehaviorTreeClass.Object;
    }
}

void APrototypeAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    auto BlackboardComponent = GetBlackboardComponent();
    if (UseBlackboard(BlackBoardAsset, BlackboardComponent))
    {
        BlackboardComponent->SetValueAsVector(this->HomePosKey, InPawn->GetActorLocation());
        if (!RunBehaviorTree(BehaviorTreeAsset))
        {

        }
    }
    GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &APrototypeAIController::OnRepeatTimer, RepeatInterval, true);
}

void APrototypeAIController::OnUnPossess()
{
    Super::OnUnPossess();
    GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void APrototypeAIController::OnRepeatTimer()
{
    auto CurrentPawn = GetPawn();
    if (!IsValid(CurrentPawn))
    {
        return;
    }

    auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (!IsValid(NavSystem))
    {
        return;
    }

    FNavLocation NextLocation;
    if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
    }

}
