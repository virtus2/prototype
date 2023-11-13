// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeEnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "ThirdPerson/Abilities/PrototypeAttributeSet.h"
#include "ThirdPerson/PrototypeAIController.h"
#include "ThirdPerson/Abilities/PrototypeAbilitySystemComponent.h"

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

void APrototypeEnemyCharacter::BeginPlay()
{
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
