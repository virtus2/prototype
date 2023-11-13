// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypePlayerState.h"

#include "Net/UnrealNetwork.h"

#include "ThirdPerson/Character/PrototypeHeroCharacter.h"
#include "ThirdPerson/Player/PrototypePlayerController.h"
#include "ThirdPerson/Abilities/PrototypeAbilitySystemComponent.h"
#include "ThirdPerson/Abilities/PrototypeAttributeSet.h"

APrototypePlayerState::APrototypePlayerState(const FObjectInitializer& ObjectInitializer)
{
    AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UPrototypeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<UPrototypeAttributeSet>(TEXT("AttributeSet"));

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

UPrototypeAttributeSet* APrototypePlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

float APrototypePlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float APrototypePlayerState::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}

float APrototypePlayerState::GetMana() const
{
    return AttributeSet->GetMana();
}

float APrototypePlayerState::GetMaxMana() const
{
    return AttributeSet->GetMaxMana();
}

void APrototypePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
    float Health = Data.NewValue;

    TObjectPtr<APrototypeHeroCharacter> HeroCharacter = Cast<APrototypeHeroCharacter>(GetPawn());
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

    TObjectPtr<APrototypeHeroCharacter> HeroCharacter = Cast<APrototypeHeroCharacter>(GetPawn());
    if (!IsValid(HeroCharacter))
    {
        return;
    }

    // TODO: Update the HUD
    UE_LOG(LogTemp, Warning, TEXT("MaxHealthChanged - NewValue: %f, OldValue: %f"), Data.NewValue, Data.OldValue);
}

void APrototypePlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
    float Mana = Data.NewValue;

    TObjectPtr<APrototypeHeroCharacter> HeroCharacter = Cast<APrototypeHeroCharacter>(GetPawn());
    if (!IsValid(HeroCharacter))
    {
        return;
    }

    // TODO: Update the HUD
    UE_LOG(LogTemp, Warning, TEXT("ManaChanged - NewValue: %f, OldValue: %f"), Data.NewValue, Data.OldValue);
}
void APrototypePlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
    float MaxMana = Data.NewValue;

    TObjectPtr<APrototypeHeroCharacter> HeroCharacter = Cast<APrototypeHeroCharacter>(GetPawn());
    if (!IsValid(HeroCharacter))
    {
        return;
    }

    // TODO: Update the HUD
    UE_LOG(LogTemp, Warning, TEXT("MaxManaChanged - NewValue: %f, OldValue: %f"), Data.NewValue, Data.OldValue);
}

void APrototypePlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &APrototypePlayerState::HealthChanged);
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