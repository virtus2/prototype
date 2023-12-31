// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeCharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "ThirdPerson/Ability/PrototypeAttributeSet.h"
#include "ThirdPerson/Ability/PrototypeGameplayAbility.h"
#include "ThirdPerson/Ability/PrototypeAbilitySystemComponent.h"


// Sets default values
APrototypeCharacterBase::APrototypeCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
}

float APrototypeCharacterBase::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float APrototypeCharacterBase::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float APrototypeCharacterBase::GetMana() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMana();
	}
	return 0.0f;
}

float APrototypeCharacterBase::GetMaxMana() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxMana();
	}
	return 0.0f;
}

// Called when the game starts or when spawned
void APrototypeCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APrototypeCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrototypeCharacterBase::InitAbilityActorInfo()
{
}

void APrototypeCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() /* || AbilitySystemComponent->bCharacterAbilitiesGiven*/)
	{
		return;
	}

	UPrototypeAbilitySystemComponent* PrototypeASC = CastChecked<UPrototypeAbilitySystemComponent>(AbilitySystemComponent);
	PrototypeASC->AddCharacterAbilities(CharacterAbilities);

	// AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void APrototypeCharacterBase::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1 /* GetCharacterLevel() */, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void APrototypeCharacterBase::AddStartupEffects()
{
}

void APrototypeCharacterBase::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void APrototypeCharacterBase::SetMana(float Mana)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetMana(Mana);
	}
}
