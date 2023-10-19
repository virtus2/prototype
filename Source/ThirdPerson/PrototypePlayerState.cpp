// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypePlayerState.h"

#include "Net/UnrealNetwork.h"

#include "PTHeroCharacter.h"
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

UPrototypeAttributeSet* APrototypePlayerState::GetAttributeSetBase() const
{
    return AttributeSetBase;
}

float APrototypePlayerState::GetHealth() const
{
    return AttributeSetBase->GetHealth();
}

float APrototypePlayerState::GetMaxHealth() const
{
    return AttributeSetBase->GetMaxHealth();
}

void APrototypePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
    float Health = Data.NewValue;

    TObjectPtr<APTHeroCharacter> HeroCharacter = Cast<APTHeroCharacter>(GetPawn());
    if (!IsValid(HeroCharacter))
    {
        return;
    }

    // TODO: Update the HUD
    UE_LOG(LogTemp, Warning, TEXT("HealthChanged - NewValue: %f, OldValue: %f"), Data.NewValue, Data.OldValue);
}

void APrototypePlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
    float MaxHealth = Data.NewValue;

    TObjectPtr<APTHeroCharacter> HeroCharacter = Cast<APTHeroCharacter>(GetPawn());
    if (!IsValid(HeroCharacter))
    {
        return;
    }

    // TODO: Update the HUD
    UE_LOG(LogTemp, Warning, TEXT("MaxHealthChanged - NewValue: %f, OldValue: %f"), Data.NewValue, Data.OldValue);
}

void APrototypePlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &APrototypePlayerState::HealthChanged);
    }
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