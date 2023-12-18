// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeEnemyCharacter.h"

#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ThirdPerson/PrototypeAIController.h"
#include "ThirdPerson/Item/PrototypeItem.h"
#include "ThirdPerson/Item/PrototypeItemGenerator.h"
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
	GameMode->ItemGenerator->SpawnItemsAt(TreasureClass, MonsterLevel, GetActorLocation());
}

void APrototypeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// TEST: 몬스터 아이템 드랍 코드
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
