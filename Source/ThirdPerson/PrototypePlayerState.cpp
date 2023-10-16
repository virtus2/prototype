// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypePlayerState.h"

#include "Net/UnrealNetwork.h"
#include "PrototypePlayerController.h"
#include "PrototypeAbilitySystemComponent.h"
#include "PrototypeAttributeSet.h"

APrototypePlayerState::APrototypePlayerState(const FObjectInitializer& ObjectInitializer)
{
    AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UPrototypeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSetBase = CreateDefaultSubobject<UPrototypeAttributeSet>(TEXT("AttributeSetBase"));

    // PlayerState의 NetUpdateFrequency 기본 값은 매우 낮기 때문에, 어빌리티 시스템에 렉을 유발할 수 있다.
    // 따라서 캐릭터의 값인 100과 같게 조정해준다. 하지만 1초에 100번 업데이트는 상용 게임에서 다소 높은 값일 수 있다.
    // 나중에 적당히 수정해주면 된다.
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

void APrototypePlayerState::BeginPlay()
{
    Super::BeginPlay();
}

void APrototypePlayerState::PreInitializeComponents()
{
    Super::PreInitializeComponents();
}

void APrototypePlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void APrototypePlayerState::ClientInitialize(AController* C)
{
    Super::ClientInitialize(C);
}

void APrototypePlayerState::OnDeactivated()
{
    Super::OnDeactivated();
}

void APrototypePlayerState::OnReactivated()
{
    Super::OnReactivated();
}

void APrototypePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
