// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"  
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "ThirdPerson/PrototypeAbilitySystemComponent.h"
#include "ThirdPerson/PrototypeAnimInstance.h"
#include "ThirdPerson/PrototypeAIController.h"
#include "ThirdPerson/PrototypeAttributeSet.h"
#include "ThirdPerson/PrototypePlayerState.h"

APrototypeCharacter::APrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetCapsuleComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);

    AIControllerClass = APrototypeAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    AttributeSet = CreateDefaultSubobject<UPrototypeAttributeSet>(TEXT("AttributeSet"));
}

void APrototypeCharacter::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("%d"), AttributeSet->GetHealth());
}

void APrototypeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APrototypeCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    HitScanLineTrace();
}

void APrototypeCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    TObjectPtr<APrototypePlayerState> PrototypePS = GetPlayerState<APrototypePlayerState>();
    if (!IsValid(PrototypePS))
    {
        UE_LOG(LogTemp, Warning, TEXT("PossessedBy Failed: Player State is null"));
        return;
    }

    auto PrototypeASC = Cast<UPrototypeAbilitySystemComponent>(PrototypePS->GetAbilitySystemComponent());
    if(!IsValid(PrototypeASC))
    {
        UE_LOG(LogTemp, Warning, TEXT("PossessedBy Failed: AbilitySystemComponent is null"));
        return;
    }

    PrototypeASC->InitAbilityActorInfo(PrototypePS, this);
    // If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
    // For now assume possession = spawn/respawn.
    InitializeAttributes();
}

void APrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    auto PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController)
    {
        return;
    }

    auto EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!EnhancedInputSubsystem)
    {
        return;
    }
    EnhancedInputSubsystem->AddMappingContext(InputMapping, 0);

    auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Look);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APrototypeCharacter::JumpStart);
    EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APrototypeCharacter::JumpStop);
    EnhancedInputComponent->BindAction(BasicFireAction, ETriggerEvent::Started, this, &APrototypeCharacter::BasicFireStart);
    EnhancedInputComponent->BindAction(BasicFireAction, ETriggerEvent::Completed, this, &APrototypeCharacter::BasicFireStop);
    EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &APrototypeCharacter::SkillUseStart);
    EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Completed, this, &APrototypeCharacter::SkillUseStop);
}

void APrototypeCharacter::InitializeAttributes()
{
    TObjectPtr<APrototypePlayerState> PrototypePS = GetPlayerState<APrototypePlayerState>();
    if (!IsValid(PrototypePS))
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeAttributes Failed: Player State is null"));
        return;
    }

    auto PrototypeASC = Cast<UPrototypeAbilitySystemComponent>(PrototypePS->GetAbilitySystemComponent());
    if (!IsValid(PrototypeASC))
    {
        UE_LOG(LogTemp, Warning, TEXT("InitializeAttributes Failed: AbilitySystemComponent is null"));
        return;
    }

    PrototypeASC
}

void APrototypeCharacter::Move(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    FRotator CharacterRotation = GetControlRotation();

    // Left / Right
    FRotator RightRotation = FRotator(0.0f, CharacterRotation.Yaw, CharacterRotation.Roll);
    FVector RightDirection = UKismetMathLibrary::GetRightVector(RightRotation);
    AddMovementInput(RightDirection, AxisValues.X);
    
    // Forward / Backward
    FRotator ForwardRotation = FRotator(0.0f, CharacterRotation.Yaw, 0.0f);
    FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(ForwardRotation);
    AddMovementInput(ForwardDirection, AxisValues.Y);
}

void APrototypeCharacter::Look(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    AddControllerYawInput(AxisValues.X);
    AddControllerPitchInput(AxisValues.Y);
}

void APrototypeCharacter::JumpStart(const FInputActionValue& Value)
{
    Jump();
}

void APrototypeCharacter::JumpStop(const FInputActionValue& Value)
{
    StopJumping();
}

void APrototypeCharacter::BasicFireStart(const FInputActionValue& Value)
{
    bBasicFireStarted = true;
    TryBasicFire();
}

void APrototypeCharacter::BasicFireStop(const FInputActionValue& Value)
{
    bBasicFireStarted = false;
}

void APrototypeCharacter::SkillUseStart(const FInputActionValue& Value)
{
}

void APrototypeCharacter::SkillUseStop(const FInputActionValue& Value)
{
}

void APrototypeCharacter::TryBasicFire()
{
    if (bBasicFireStarted)
    {
        if (bBasicFireReady)
        {
            BasicFire();
        }
    }
}

void APrototypeCharacter::BasicFire()
{
 
    HitScanLineTrace();
    
    // 타이머 다시 설정
    bBasicFireReady = false;
    GetWorldTimerManager().SetTimer(BasicFireTimerHandle, this, &APrototypeCharacter::BasicFireTimerFinished, BasicFireDelay);

    // 애니메이션 몽타주 재생
    TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && BasicFireAnimMontage)
    {
        AnimInstance->Montage_Play(BasicFireAnimMontage, 1.0f / BasicFireDelay);
    }
}

void APrototypeCharacter::BasicFireTimerFinished()
{
    bBasicFireReady = true;
    if (bBasicFireStarted)
    {
        TryBasicFire();
    }
}

void APrototypeCharacter::ComboAttackSave()
{
    if (bSaveAttack)
    {
        bSaveAttack = false;
        switch (AttackCount)
        {
        case 0:
            AttackCount = 1;
            break;
        case 1:
            AttackCount = 2;
            break;
        case 2:
            AttackCount = 0;
            break;
        default:
            break;
        }
    }
}

void APrototypeCharacter::ResetCombo()
{
    AttackCount = 0;
    bSaveAttack = false;

}

void APrototypeCharacter::HitScanLineTrace()
{
    TObjectPtr<UWorld> World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    auto PlayerController = Cast<APlayerController>(Controller);
    if (!IsValid(PlayerController))
    {
        return;
    }

    auto PlayerCameraManager = PlayerController->PlayerCameraManager;
    if (!IsValid(PlayerCameraManager))
    {
        return;
    }

    // 라인트레이스를 쏘는 캐릭터는 제외한다.
    FCollisionQueryParams LineTraceParams;
    LineTraceParams.AddIgnoredActor(this);

    // 1. 조준점의 방향으로 라인트레이스를 해서 충돌하는 첫번째 캐릭터를 구한다.
    FVector AimLineTraceStart = PlayerCameraManager->GetCameraLocation(); 
    FVector AimLineTraceEnd = AimLineTraceStart + PlayerCameraManager->GetActorForwardVector() * 10000.0f; 

    FHitResult AimLineTraceHitResult;
    bool bIsAimLineTraceHit = World->LineTraceSingleByChannel(
        AimLineTraceHitResult,
        AimLineTraceStart,
        AimLineTraceEnd,
        ECC_GameTraceChannel1, // Custom Collision Channel: HitScan. See DefaultEngine.ini 
        LineTraceParams
    );

    bool bIsAimTraceHitTooFar = false;
    TObjectPtr<ACharacter> AimLineTraceHitCharacter;
    if (bIsAimLineTraceHit)
    {
        AimLineTraceHitCharacter = Cast<ACharacter>(AimLineTraceHitResult.GetActor());
        if (IsValid(AimLineTraceHitCharacter))
        {
            // 2. 히트된 캐릭터가 뒤나 옆에 있는지 판단한다.
            // (라인트레이스 시작지점을 캐릭터 거리만큼 더해줘도 되긴 할 것 같다.)
            FVector ActorLocationToImpactPoint = (AimLineTraceHitResult.ImpactPoint - GetActorLocation());
            FVector HitDirection = ActorLocationToImpactPoint.GetSafeNormal();
            float dot = GetActorForwardVector().GetSafeNormal().Dot(HitDirection);
            if (dot <= 0.3f) 
            {
                return;
            }

            // 3. 히트된 캐릭터까지의 거리가 먼지 판단한다.
            float SqrDistanceToHitCharacter = ActorLocationToImpactPoint.SizeSquared();
            if (SqrDistanceToHitCharacter >= 17500.0f)
            {
                bIsAimTraceHitTooFar = true;
            }
        }
    }

    // 4. 총구 방향에서 라인트레이스
    FVector MuzzleLineTraceStart = GetMesh()->GetSocketLocation(TEXT("hand_lSocket"));
    FVector MuzzleLineTraceEnd;
    if (bIsAimLineTraceHit)
    {
        // 4-1. 조준점 라인 트레이스가 충돌했으면 총구에서 충돌지점으로 라인트레이스를 한다.
        FVector MuzzleLineTraceDirection = (AimLineTraceHitResult.ImpactPoint - MuzzleLineTraceStart);
        MuzzleLineTraceDirection.Normalize();
        MuzzleLineTraceEnd = MuzzleLineTraceStart + MuzzleLineTraceDirection * 10000.0f;
    }
    else
    {
        // 4-2. 조준점 라인 트레이스가 충돌하지 않았으면 조준점 라인트레이스의 TraceEnd를 사용한다.
        //      이때 길이를 그대로 사용하면 멀리 있는 캐릭터도 총구 방향에 따라 맞출 수 있기 때문에,
        //      게임 플레이 경험을 위해서 짧게 조정해준다.
        MuzzleLineTraceEnd = AimLineTraceHitResult.TraceEnd * 0.000001f;
    }

    FHitResult MuzzleLineTraceHitResult;
    bool bIsMuzzleLineTraceHit = World->LineTraceSingleByChannel(
        MuzzleLineTraceHitResult,
        MuzzleLineTraceStart,
        MuzzleLineTraceEnd,
        ECC_GameTraceChannel1, // Custom Collision Channel: HitScan. See DefaultEngine.ini 
        LineTraceParams
    );

    TObjectPtr<ACharacter> MuzzleLineTraceHitCharacter;
    if (bIsMuzzleLineTraceHit)
    {
        MuzzleLineTraceHitCharacter = Cast<ACharacter>(MuzzleLineTraceHitResult.GetActor());
    }

    // 5. 조준점 라인트레이스로 맞은 캐릭터가 없는 경우 총구 라인트레이스로 히트 판정을 내린다. 
    //    근접 상황에서는 조준점 라인트레이스는 맞지 않고 총구 라인트레이스만 맞기 때문.
    if (IsValid(MuzzleLineTraceHitCharacter) && !IsValid(AimLineTraceHitCharacter))
    {
        DrawDebugLine(World, MuzzleLineTraceStart, MuzzleLineTraceEnd, FColor(0, 255, 0));
        UE_LOG(LogTemp, Warning, TEXT("Close Hit: %s"), *MuzzleLineTraceHitCharacter.GetFullName());
    }

    // 6. 조준점 라인트레이스 히트 결과 거리가 멀면 캐릭터 라인트레이스와 결과가 같을 때 히트 판정을 내린다.
    //    벽 뒤에서 쏠 때 총구가 벽에 막히게 하기 위함.
    if(bIsAimTraceHitTooFar)
    {
        if (IsValid(AimLineTraceHitCharacter) && IsValid(MuzzleLineTraceHitCharacter))
        {
            DrawDebugLine(World, AimLineTraceStart, AimLineTraceEnd, FColor(0, 255, 0));
            DrawDebugLine(World, MuzzleLineTraceStart, MuzzleLineTraceEnd, FColor(0, 255, 0));
            UE_LOG(LogTemp, Warning, TEXT("Far Hit: %s"), *MuzzleLineTraceHitCharacter.GetFullName());
        }
    }
}


