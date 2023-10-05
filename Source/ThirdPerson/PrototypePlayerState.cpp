// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypePlayerState.h"

#include "PrototypeAbilitySystemComponent.h"

APrototypePlayerState::APrototypePlayerState(const FObjectInitializer& ObjectInitializer)
{
    AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UPrototypeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // 어빌리티 시스템은 자주 업데이트 해줘야한다...?
    NetUpdateFrequency = 100.0f;
}

APrototypePlayerController* APrototypePlayerState::GetPrototypePlayerController() const
{
    return Cast<APrototypePlayerController>(GetOwner());
}

UAbilitySystemComponent* APrototypePlayerState::GetAbilitySystemComponent() const
{
    return GetPrototypeAbilitySystemComponent();
}

void APrototypePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
