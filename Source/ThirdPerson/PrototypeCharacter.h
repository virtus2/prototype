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
struct FInputActionValue;
struct FTimerHandle;

USTRUCT(BlueprintType)
struct FPrototypeCharacterStatus : public FTableRowBase
{
    GENERATED_BODY()

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

protected:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void BeginPlay() override;

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

    void LoadStatusFromTable();

private:


public:

protected:
    UPROPERTY(EditAnywhere)
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> BasicFireAnimMontage;

    /*
        Enhanced Input...
    */
    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> BasicFireAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SkillAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveRAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveEAction;

    UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
    TObjectPtr<UInputAction> SpecialMoveQAction;

    /*
        Character Status... 
    */
    UPROPERTY(EditDefaultsOnly, Category = "Character Status")
    FPrototypeCharacterStatus CharacterStatus;

    UPROPERTY(EditDefaultsOnly, Category ="Character Status")
    float CurrentHealthPoint;

    UPROPERTY(VisibleAnywhere, Category = "Character Status|Combat")
    bool bSaveAttack;

    UPROPERTY(VisibleAnywhere, Category = "Character Status|Combat")
    bool bResetCombo;

    UPROPERTY(VisibleAnywhere, Category="Character Status|Combat")
    bool bBasicFireStarted;

    UPROPERTY(VisibleAnywhere, Category = "Character Status|Combat")
    bool bBasicFireReady = true;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character Status|Combat")
    float BasicFireDelay;

    FTimerHandle BasicFireTimerHandle;

    int AttackCount;


private:


};
