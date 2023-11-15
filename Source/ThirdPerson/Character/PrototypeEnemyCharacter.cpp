// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeEnemyCharacter.h"

#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ThirdPerson/PrototypeAIController.h"
#include "ThirdPerson/Data/TreasureClass.h"
#include "ThirdPerson/Game/PrototypeGameModeBase.h"
#include "ThirdPerson/Ability/PrototypeAttributeSet.h"
#include "ThirdPerson/Ability/PrototypeAbilitySystemComponent.h"

APrototypeEnemyCharacter::APrototypeEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UPrototypeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UPrototypeAttributeSet>("AttributeSet");
}

void APrototypeEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}

	PrototypeAIController = Cast<APrototypeAIController>(NewController);
	// TODO: BlackboardComponent, BehaviorTree 초기화
}

void APrototypeEnemyCharacter::SpawnTreasure()
{
	const TObjectPtr<APrototypeGameModeBase> GameMode = Cast<APrototypeGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GameMode))
	{
		return;
	}
	
	// TODO: 아이템 만들어주는 클래스를 따로 만들어서 옮긴다
	const FString Context;
	FTreasureClass* TC = GameMode->TreasureClassDataTable->FindRow<FTreasureClass>(TreasureClass.GetTagName(), Context);
	TArray<FTreasure> PickedTreasures;
	// TODO: 음수 일경우 확정적으로 N개를 드랍한다.
	if (TC->NumPicks < 0)
	{

	}
	else
	{
		for (int picksLeft = TC->NumPicks; picksLeft > 0; picksLeft--)
		{
			int ProbTotal = TC->FreqNoDrop;
			for (const auto& Treasure : TC->Treasures)
			{
				ProbTotal += Treasure.Probability;
			}
			
			int RandomProb = FMath::RandRange(0, ProbTotal);
			int ProbRollSum = 0;
			for (const auto& Treasure : TC->Treasures)
			{
				ProbRollSum += Treasure.Probability;
				if (ProbRollSum >= RandomProb)
				{
					PickedTreasures.Add(Treasure);
					break;
				}
			}
		}
	}

	for (const auto& Treasure : PickedTreasures)
	{
		// TreasureClass가 아닌 아이템 타입일 경우 해당 아이템을 드롭
		if (Treasure.Treasure.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Type")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item.Type"));
		}
		// TreasureClass일 경우
		else if (Treasure.Treasure.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Treasure")))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item.Treasure"));
		}
	}
}

void APrototypeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnTreasure();
}

void APrototypeEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	if (HasAuthority())
	{
		InitializeAttributes();
	}
	// OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void APrototypeEnemyCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();
}
