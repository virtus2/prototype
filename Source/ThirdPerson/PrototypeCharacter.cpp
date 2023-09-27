// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"    
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

APrototypeCharacter::APrototypeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(GetCapsuleComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->AttachToComponent(SpringArmComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void APrototypeCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APrototypeCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    HitScanLineTrace();
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
    bBasicFireReady = false;
    GetWorldTimerManager().SetTimer(BasicFireTimerHandle, this, &APrototypeCharacter::BasicFireTimerFinished, BasicFireDelay);
    UE_LOG(LogTemp, Warning, TEXT("Fire!!"));

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
    FVector2D ViewportSize;
    if (!IsValid(GEngine))
    {
        return;
    }
    if (!IsValid(GEngine->GameViewport))
    {
        return;
    }

    // 1. 조준점(스크린의 정 가운데)의 월드 좌표와 방향벡터를 구한다.
    GEngine->GameViewport->GetViewportSize(ViewportSize);
    FVector2D ViewportCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
    FVector AimWorldPosition;
    FVector AimWorldDirection;
    bool DeprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(Cast<APlayerController>(Controller), ViewportCenter, AimWorldPosition, AimWorldDirection);
    if (!DeprojectionSuccess)
    {
        return;
    }

    auto World = GetWorld();
    if (!IsValid(World))
    {
        return;
    }

    // 2. 조준점의 방향으로 라인트레이스를 해서 충돌하는 첫번째 캐릭터를 구한다.
    FVector AimLineTraceStart = AimWorldPosition + AimWorldDirection; 
    FVector AimLineTraceEnd = AimLineTraceStart + AimWorldDirection * 1500.0f;

    FCollisionQueryParams AimLineTraceParams;
    AimLineTraceParams.AddIgnoredActor(this);

    FHitResult AimLineTraceHitResult;
    bool AimLineTraceHit = World->LineTraceSingleByChannel(
        AimLineTraceHitResult,
        AimLineTraceStart,
        AimLineTraceEnd,
        ECC_GameTraceChannel1, // Custom Collision Channel: HitScan. See DefaultEngine.ini 
        AimLineTraceParams
    );

    bool bIsTraceHitTooClose = false;
    TObjectPtr<ACharacter> AimLineTraceHitCharacter;
    if (AimLineTraceHit)
    {
        AimLineTraceHitCharacter = Cast<ACharacter>(AimLineTraceHitResult.GetActor());
        if (IsValid(AimLineTraceHitCharacter))
        {
            // 3. 히트된 캐릭터가 뒤나 옆에 있는지 판단한다.
            FVector ActorLocationToImpactPoint = (AimLineTraceHitResult.ImpactPoint - GetActorLocation());
            FVector HitDirection = ActorLocationToImpactPoint.GetSafeNormal();
            float dot = GetActorForwardVector().GetSafeNormal().Dot(HitDirection);
            // UE_LOG(LogTemp, Warning, TEXT("Dot:%f"), dot)
            // TODO: 적당한 값을 정하자
            if (dot <= 0.3f) 
            {
                return;
            }

            // 4. 히트된 캐릭터까지의 거리가 가까운지 판단한다.
            float SqrDistanceToHitCharacter = ActorLocationToImpactPoint.SizeSquared();
            // UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), SqrDistanceToHitCharacter)
            if (SqrDistanceToHitCharacter < 17500.0f)
            {
                DrawDebugLine(World, GetActorLocation(), AimLineTraceHitResult.ImpactPoint, FColor(255, 0, 0));
                bIsTraceHitTooClose = true;
            }
        }
    }

    // 5. 캐릭터에서 충돌지점으로 라인트레이스를 한다.
    FVector CharacterSightLineTraceStart = GetActorLocation();
    FVector CharacterSightLineTraceDirection = (AimLineTraceHitResult.ImpactPoint - CharacterSightLineTraceStart);
    CharacterSightLineTraceDirection.Normalize();
    FVector CharacterSightLineTraceEnd = CharacterSightLineTraceStart + CharacterSightLineTraceDirection * 1500.0f;

    FCollisionQueryParams CharacterLineTraceParams;
    CharacterLineTraceParams.AddIgnoredActor(this);

    FHitResult CharacterSightLineTraceHitResult;
    bool CharacterLineTraceHit = World->LineTraceSingleByChannel(
        CharacterSightLineTraceHitResult,
        CharacterSightLineTraceStart,
        CharacterSightLineTraceEnd,
        ECC_GameTraceChannel1, // Custom Collision Channel: HitScan. See DefaultEngine.ini 
        CharacterLineTraceParams
    );

    TObjectPtr<ACharacter> CharacterLineTraceHitCharacter;
    if (CharacterLineTraceHit)
    {
        CharacterLineTraceHitCharacter = Cast<ACharacter>(CharacterSightLineTraceHitResult.GetActor());
    }

    // 6. 충돌한 캐릭터와 거리가 가까우면 캐릭터 라인트레이스만으로 히트 판정을 내린다.
    if (bIsTraceHitTooClose)
    {
        if (IsValid(CharacterLineTraceHitCharacter))
        {
            DrawDebugLine(World, CharacterSightLineTraceStart, CharacterSightLineTraceEnd, FColor(0, 255, 0));
            UE_LOG(LogTemp, Warning, TEXT("%s"), *CharacterLineTraceHitCharacter.GetFullName());
        }
    }
    else
    {
        // 7. 거리가 멀면 조준점, 캐릭터 라인트레이스 결과가 같을때 히트 판정을 내린다. (벽 뒤에서 쏠 때 총구가 벽에 막히게 하기 위함)
        if (IsValid(AimLineTraceHitCharacter) && IsValid(CharacterLineTraceHitCharacter))
        {
            DrawDebugLine(World, AimLineTraceStart, AimLineTraceEnd, FColor(0, 255, 0));
            DrawDebugLine(World, CharacterSightLineTraceStart, CharacterSightLineTraceEnd, FColor(0, 255, 0));
            UE_LOG(LogTemp, Warning, TEXT("%s"), *CharacterLineTraceHitCharacter.GetFullName());
        }
    }

    // TODO: 초근접 상황에서 조준점 라인트레이스는 안맞았는데 캐릭터 총구에서 발사해서 맞는 경우도 있음... 수정해야됨

}


