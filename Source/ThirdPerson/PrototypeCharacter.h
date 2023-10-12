// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototypeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class UPrototypeAnimInstance;
class UPrototypeAttributeSet;
class UGameplayEffect;
class UPrototypeAbilitySet;
struct FInputActionValue;
struct FTimerHandle;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FPrototypeCharacterStatus : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FPrototypeCharacterStatus()
        : MaxHealthPoint(1000.0f)
        , AttackSpeed(1.0f)
        , CharacterIcon()
    {}

public:
    /** 캐릭터의 최대 체력 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHealthPoint;

    /** 캐릭터의 공격 속도 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackSpeed;

    /** 캐릭터의 아이콘 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture> CharacterIcon;

    /** 캐릭터의 스켈레탈 메쉬 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<USkeletalMesh> CharacterMesh;

    /** 캐릭터의 애니메이션 블루프린트 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UPrototypeAnimInstance> AnimInstance;

    /** 캐릭터의 기본 공격 몽타주 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UAnimMontage> BasicFireAnimMontage;
};

UCLASS()
class THIRDPERSON_API APrototypeCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APrototypeCharacter();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Initialize the Character's attributes. Must run on Server but we run it on Client too
    // so that we don't have to wait. The Server's replication to the Client won't matter since
    // the values should be the same.
    virtual void InitializeAttributes();
    /*
    virtual void OnAbilitySystemInitialized();
    virtual void OnAbilitySystemUninitialized();

    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    
    virtual void OnRep_Controller() override;
    virtual void OnRep_PlayerState() override;
    */

    virtual void Move(const FInputActionValue& Value);
    virtual void Look(const FInputActionValue& Value);
    virtual void JumpStart(const FInputActionValue& Value);
    virtual void JumpStop(const FInputActionValue& Value);

    virtual void BasicFireStart(const FInputActionValue& Value);
    virtual void BasicFireStop(const FInputActionValue& Value);
    virtual void SkillUseStart(const FInputActionValue& Value);
    virtual void SkillUseStop(const FInputActionValue& Value);

    virtual void TryBasicFire();
    virtual void BasicFire();
    virtual void BasicFireTimerFinished();
    virtual void ComboAttackSave();
    virtual void ResetCombo();

    void HitScanLineTrace();

    void HandleHealthChanged(const FOnAttributeChangeData& Data);

private:


public:

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(EditAnywhere, Category="Prototype Character|Animation")
    TObjectPtr<UAnimMontage> BasicFireAnimMontage;

    /*
        Enhanced Input...
    */
    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> BasicFireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SkillAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveRAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveEAction;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveQAction;

    /*
        Character Status... 
    */
    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Character Status")
    FPrototypeCharacterStatus CharacterStatus;

    UPROPERTY(EditDefaultsOnly, Category ="Prototype Character|Character Status")
    float CurrentHealthPoint;

    UPROPERTY(VisibleAnywhere, Category = "Prototype Character|Character Status|Combat")
    bool bSaveAttack;

    UPROPERTY(VisibleAnywhere, Category = "Prototype Character|Character Status|Combat")
    bool bResetCombo;

    UPROPERTY(VisibleAnywhere, Category="Prototype Character|Character Status|Combat")
    bool bBasicFireStarted;

    UPROPERTY(VisibleAnywhere, Category = "Prototype Character|Character Status|Combat")
    bool bBasicFireReady = true;
    
    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Character Status|Combat")
    float BasicFireDelay;

    FTimerHandle BasicFireTimerHandle;

    int AttackCount;


    /*
     * 어빌리티 시스템 테스트
     */

    // Default attributes for a character for initializing on spawn/respawn.
    // This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Prototype Character|Ability")
    TSubclassOf<UGameplayEffect> DefaultAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Prototype Character|Ability")
    TSubclassOf<UPrototypeAttributeSet> AttributeSetClass;

    UPROPERTY()
    UPrototypeAttributeSet* AttributeSet;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Prototype Character|Ability")
    TArray<TObjectPtr<UPrototypeAbilitySet>> AbilitySets;


private:


};
