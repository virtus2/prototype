// Fill out your copyright notice in the Description page of Project Settings.


#include "PTCharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "PrototypeGameplayAbility.h"
#include "PrototypeAbilitySystemComponent.h"


// Sets default values
APTCharacterBase::APTCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void APTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APTCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() /* || AbilitySystemComponent->bCharacterAbilitiesGiven*/)
	{
		return;
	}

	for (TSubclassOf<UPrototypeGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		// TODO: 어빌리티 레벨과 인풋ID를 어떻게 해야할까
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, -1, this));
	}

	// AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void APTCharacterBase::InitializeAttributes()
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

void APTCharacterBase::AddStartupEffects()
{
}

