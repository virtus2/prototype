// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeHeroCharacter.h"

#include "GameplayTagContainer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ThirdPerson/Player/PrototypePlayerState.h"
#include "ThirdPerson/Player/PrototypePlayerController.h"
#include "ThirdPerson/Character/PrototypeInventoryComponent.h"
#include "ThirdPerson/Abilities/PrototypeAttributeSet.h"
#include "ThirdPerson/Abilities/PrototypeAbilitySystemComponent.h"
#include "ThirdPerson/Input/PrototypeEnhancedInputComponent.h"

APrototypeHeroCharacter::APrototypeHeroCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetCapsuleComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SetRelativeLocation(FVector(0, 0, 68.492264));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    FollowCamera->SetupAttachment(SpringArmComponent);
    FollowCamera->FieldOfView = 80.0f;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    Inventory = CreateDefaultSubobject<UPrototypeInventoryComponent>(TEXT("Inventory"));
}

void APrototypeHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

    auto EnhancedInputComponent = Cast<UPrototypeEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }

    // EnhancedInput Bind Actions
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrototypeHeroCharacter::Move);
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrototypeHeroCharacter::Look);
    EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &APrototypeHeroCharacter::ActivateInventory);
    // Gameplay Ability System Bind Input
    EnhancedInputComponent->BindAbilityActions(InputConfig, this, &APrototypeHeroCharacter::AbilityInputTagPressed, &APrototypeHeroCharacter::AbilityInputTagReleased, &APrototypeHeroCharacter::AbilityInputTagHeld);
}

void APrototypeHeroCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FHitResult HitResult;
    AimLineTrace(/* out */HitResult);
    if (HitResult.bBlockingHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("AimLineTrace Hitted By: %s"), *(HitResult.GetActor()->GetFullName()));
    }
}

void APrototypeHeroCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitAbilityActorInfo();
    InitializeAttributes();
    AddCharacterAbilities();
    SetHealth(GetMaxHealth());
    SetMana(GetMaxMana());

    TObjectPtr<APrototypePlayerController> PC = Cast<APrototypePlayerController>(GetController());
    if (!IsValid(PC))
    {
        return;
    }
    PC->CreateHUD();
}

void APrototypeHeroCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    InitAbilityActorInfo();
    TObjectPtr<APrototypePlayerController> PC = Cast<APrototypePlayerController>(GetController());
    if (!IsValid(PC))
    {
        return;
    }
    PC->CreateHUD();
}

void APrototypeHeroCharacter::Move(const FInputActionValue& Value)
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

void APrototypeHeroCharacter::Look(const FInputActionValue& Value)
{
    auto AxisValues = Value.Get<FInputActionValue::Axis2D>();
    AddControllerYawInput(AxisValues.X);
    AddControllerPitchInput(AxisValues.Y);
}

void APrototypeHeroCharacter::JumpStart(const FInputActionValue& Value)
{
    Jump();
}

void APrototypeHeroCharacter::JumpStop(const FInputActionValue& Value)
{
    StopJumping();
}

void APrototypeHeroCharacter::InitAbilityActorInfo()
{
    Super::InitAbilityActorInfo();

    TObjectPtr<APrototypePlayerState> PS = GetPlayerState<APrototypePlayerState>();
    if (!IsValid(PS))
    {
        return;
    }

    AbilitySystemComponent = Cast<UPrototypeAbilitySystemComponent>(PS->GetAbilitySystemComponent());
    AbilitySystemComponent->InitAbilityActorInfo(PS, this);
    AttributeSet = PS->GetAttributeSet();
}

void APrototypeHeroCharacter::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void APrototypeHeroCharacter::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void APrototypeHeroCharacter::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!AbilitySystemComponent.IsValid())
    {
        return;
    }
    AbilitySystemComponent->AbilityInputTagHeld(InputTag);
}

void APrototypeHeroCharacter::ActivateInventory()
{
    Inventory->ActivateInventoryWidget();
}

void APrototypeHeroCharacter::AimLineTrace(FHitResult& HitResult)
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

    // 1. 조준점의 방향으로 라인트레이스를 해서 충돌하는 첫번째 액터를 구한다.
    FVector AimLineTraceStart = PlayerCameraManager->GetCameraLocation();
    FVector AimLineTraceEnd = AimLineTraceStart + PlayerCameraManager->GetActorForwardVector() * 10000.0f;

    FHitResult AimLineTraceHitResult;
    bool bIsAimLineTraceHit = World->LineTraceSingleByChannel(
        AimLineTraceHitResult,
        AimLineTraceStart,
        AimLineTraceEnd,
        ECC_WorldDynamic,
        LineTraceParams
    );

    bool bIsAimTraceHitTooFar = false;
    TObjectPtr<AActor> AimLineTraceHitActor;
    if (bIsAimLineTraceHit)
    {
        AimLineTraceHitActor = Cast<AActor>(AimLineTraceHitResult.GetActor());
        if (IsValid(AimLineTraceHitActor))
        {
            // 2. 히트된 액터가 뒤나 옆에 있는지 판단한다.
            // (라인트레이스 시작지점을 캐릭터 ~ 카메라 거리만큼 더해줘도 되긴 할 것 같다.)
            FVector ActorLocationToImpactPoint = (AimLineTraceHitResult.ImpactPoint - GetActorLocation());
            FVector HitDirection = ActorLocationToImpactPoint.GetSafeNormal();
            float dot = GetActorForwardVector().GetSafeNormal().Dot(HitDirection);
            if (dot <= 0.3f)
            {
                return;
            }

            // 3. 히트된 액터까지의 거리가 먼지 판단한다.
            float SqrDistanceToHitActor= ActorLocationToImpactPoint.SizeSquared();
            if (SqrDistanceToHitActor >= 17500.0f)
            {
                bIsAimTraceHitTooFar = true;
            }
        }
    }

    // 4. 총구 방향에서 라인트레이스
    FVector MuzzleLineTraceStart = GetMesh()->GetSocketLocation(MuzzleSocketName);
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
        MuzzleLineTraceEnd = AimLineTraceEnd; //* 0.000001f;
    }

    FHitResult MuzzleLineTraceHitResult;
    bool bIsMuzzleLineTraceHit = World->LineTraceSingleByChannel(
        MuzzleLineTraceHitResult,
        MuzzleLineTraceStart,
        MuzzleLineTraceEnd,
        ECC_Pawn,
        LineTraceParams
    );

    TObjectPtr<AActor> MuzzleLineTraceHitActor;
    if (bIsMuzzleLineTraceHit)
    {
        MuzzleLineTraceHitActor = Cast<AActor>(MuzzleLineTraceHitResult.GetActor());
    }

    // 5. 조준점 라인트레이스로 맞은 캐릭터가 없는 경우 총구 라인트레이스로 히트 판정을 내린다. 
    //    근접 상황에서는 조준점 라인트레이스는 맞지 않고 총구 라인트레이스만 맞기 때문.
    if (IsValid(MuzzleLineTraceHitActor) && !IsValid(AimLineTraceHitActor))
    {
        HitResult = MuzzleLineTraceHitResult;
        DrawDebugLine(World, MuzzleLineTraceStart, MuzzleLineTraceEnd, FColor(255, 0, 0));
        // UE_LOG(LogTemp, Warning, TEXT("Close Hit: %s"), *MuzzleLineTraceHitActor.GetFullName());
    }

    // 6. 조준점 라인트레이스 히트 결과 거리가 멀면 캐릭터 라인트레이스와 결과가 같을 때 히트 판정을 내린다.
    //    벽 뒤에서 쏠 때 총구가 벽에 막히게 하기 위함.
    if (bIsAimTraceHitTooFar)
    {
        if (IsValid(MuzzleLineTraceHitActor) && IsValid(AimLineTraceHitActor))
        {
            HitResult = MuzzleLineTraceHitResult;
            DrawDebugLine(World, AimLineTraceStart, AimLineTraceEnd, FColor(0, 0, 255));
            DrawDebugLine(World, MuzzleLineTraceStart, MuzzleLineTraceEnd, FColor(0, 0, 255));
            // UE_LOG(LogTemp, Warning, TEXT("Far Hit: %s"), *MuzzleLineTraceHitActor.GetFullName());
        }
    }
}
