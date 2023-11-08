#include "ShooterCharacter.h"

#include "Ammo.h"
#include "Custom.h"
#include "Item.h"
#include "Weapon.h"

#include <Animation\AnimInstance.h>
#include <Camera\CameraComponent.h>
#include <Camera\PlayerCameraManager.h>
#include <Components\BoxComponent.h>
#include <Components\CapsuleComponent.h>
#include <Components\SkeletalMeshComponent.h>
#include <Components\SphereComponent.h>
#include <Components\WidgetComponent.h>
#include <DrawDebugHelpers.h>
#include <Engine\SkeletalMesh.h>
#include <Engine\SkeletalMeshSocket.h>
#include <Engine\World.h>
#include <GameFramework\CharacterMovementComponent.h>
#include <GameFramework\Pawn.h>
#include <GameFramework\SpringArmComponent.h>
#include <Kismet\GameplayStatics.h>
#include <Kismet\KismetMathLibrary.h>
#include <Particles\ParticleSystem.h>
#include <Particles\ParticleSystemComponent.h>
#include <Sound\SoundCue.h>
#include <UObject\ConstructorHelpers.h>

AShooterCharacter::AShooterCharacter()

 : PlayerMesh(nullptr)
 , SkeletalMeshContainer(nullptr)
 , CameraBoom(nullptr)
 , FollowCamera(nullptr)
 , bIsTransitioning(true)
 , bCinematicCameraSwitch(true)
 , bUseBezierCurve(true)
 , CameraArmLengthStart(200.f)
 , CameraArmLengthEnd(300.f)
 , TransitionDuration(0.2f)
 , PreviousYaw(0.f)
 , LerpedArmLength(0.f)
 , CurrentTime(0.0f)
 , BaseTurnRate(45.f)
 , BaseLookUpRate(45.f)
 , HipTurnRate(90.f)
 , HipLookUpRate(90.f)
 , AimingTurnRate(20.f)
 , AimingLookUpRate(20.f)
 , MouseHipTurnRate(1.f)
 , MouseHipLookUpRate(1.f)
 , MouseAimingTurnRate(0.8f)
 , MouseAimingLookUpRate(0.8f)
 , bIsFiringWeapon(false)
 , bDidFire(false)
 , FireSound(nullptr)
 , MuzzleFlash(nullptr)
 , ImpactParticles(nullptr)
 , BeamParticles(nullptr)
 , HipFireMontage(nullptr)
 , EquipMontage(nullptr)
 , bShowCustomDebugMessages(false)
 , bAiming(false)
 , CameraDefaultAimFOV(0.f)
 , CameraZoomedAimFOV(40.f)
 , CameraCurrentAimFOV(0.f)
 , ZoomInterpSpeed(20.f)
 , CrosshairHeight(50.f)
 , CrosshairSpreadMultiplier(0.f)
 , CrosshairVelocityFactor(0.f)
 , CrosshairInAirFactor(0.f)
 , CrosshairAimFactor(0.f)
 , CrosshairShootingFactor(0.f)
 , ShootTimeDuration(0.05f)
 , bFiringBullet(false)
 , CrosshairShootTimer(FTimerHandle())
 , bFireButtonPressed(false)
 , bShouldFire(true)
 , AutomaticFireRate(0.1f)
 , AutoFireTimer(FTimerHandle())
 , bShouldTraceForItems(false)
 , OverlappedItemCount(0)
 , TraceHitItemLastFrame(nullptr)
 , EquippedWeapon(nullptr)
 , DefaultWeaponClass(nullptr)
 , TraceHitItem(nullptr)
 , CameraInterpDistance(250.f)
 , CameraInterpElevation(65.f)
 , AmmoMap(TMap<EAmmoType, int32>())
 , Starting_9mm_Ammo(85)
 , Starting_AR_Ammo(120)
 , CombatState(ECombatState::ECS_Unoccupied)
 , ReloadMontage(nullptr)
 , ClipTransform(FTransform())
 , HandSceneComponent(nullptr)
 , bCrouching(false)
 , BaseMovementSpeed(650.f)
 , CrouchMovementSpeed(300.f)
 , CurrentCapsuleHalfHeight(0.f)
 , StandingCapsuleHalfHeight(88.f)
 , CrouchingCapsuleHalfHeight(44.f)
 , BaseGroundFriction(2.f)
 , CrouchingGroundFriction(100.f)
 , bAimingButtonPressed(false)
 , WeaponInterpComp(nullptr)
 , InterpComp1(nullptr)
 , InterpComp2(nullptr)
 , InterpComp3(nullptr)
 , InterpComp4(nullptr)
 , InterpComp5(nullptr)
 , InterpComp6(nullptr)
 , InterpLocations(TArray<FInterpLocation>())
 , PickupSoundTimer(FTimerHandle())
 , EquipSoundTimer(FTimerHandle())
 , bShouldPlayPickupSound(true)
 , bShouldPlayEquipSound(true)
 , PickupSoundResetTime(0.2f)
 , EquipSoundResetTime(0.2f)
 , Inventory(TArray<AItem *>())
 , DebugKeys(false)
 , CurrentSlotIndex(0)
 , InventoryCount(0)
 , bDebugSlotMessages(false) {

    PrimaryActorTick.bCanEverTick = true;

    DefaultConstructor_SetupMesh();
    DefaultConstructor_SetCharacter();
    DefaultConstructor_CameraBoom();
    DefaultConstructor_FollowCamera();
    DefaultConstructor_SetCombatCues();

    // Create Hand Scene Component
    HandSceneComponent = CDSubObj<USceneComponent>(L"HandSceneComp");

    GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

    DefaultConstructor_InterpComponents();
}

void AShooterCharacter::BeginPlay() {

    Super::BeginPlay();

    SetupFollowCamera();

    // Spawn Default Weapon and equip it
    EquipWeapon(SpawnDefaultWeapon());
    Inventory.Add(EquippedWeapon);
    EquippedWeapon->SetSlotIndex(0);
    EquippedWeapon->DisableCustomDepth();
    EquippedWeapon->DisableGlowMaterial();

    InitializeAmmoMap();
    InitializeInterpLocations();

    UpdateSlotsItens();
}

void AShooterCharacter::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);

    if (bIsTransitioning) StartCameraLerp(DeltaTime);

    // Relative_ControllerRotationYaw(DeltaTime);

    AimingCameraZoom(DeltaTime);

    SetsLookRates();

    CalculateCrosshairSpread(DeltaTime);

    if (bShouldTraceForItems || TraceHitItemLastFrame) TraceForItems();

    // Interpolate capsule half height based on crouching/standing
    InterpCapsuleHalfHeight(DeltaTime);

    if (bDebugSlotMessages) DebugSlotsItens();
}

void AShooterCharacter::InterpCapsuleHalfHeight(float DeltaTime) {

    float TargetCapsuleHalfHeight {

      bCrouching ? CrouchingCapsuleHalfHeight : StandingCapsuleHalfHeight};

    float GetScaledCapsule {GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
    float InterpSpeed {20.f};

    const float InterpHalfHeight {
      FMath::FInterpTo(GetScaledCapsule, TargetCapsuleHalfHeight, DeltaTime, InterpSpeed)};

    // Negative value if crouching, positive value standing
    const float DeltaCapsuleHalfHeight {InterpHalfHeight - GetScaledCapsule};

    // Offset mesh to compensate for capsule height change
    const FVector MeshOffset {0.f, 0.f, -DeltaCapsuleHalfHeight};
    GetMesh()->AddLocalOffset(MeshOffset);

    GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AShooterCharacter::StartCameraLerp(float &DeltaTime) {

    CurrentTime += DeltaTime;

    // get CurrentTime / TransitionDuration
    float CurrTmDivTransDur {CurrentTime / TransitionDuration};

    // Clamps CurrentTime / TransitionDuration
    float Alpha {FMath::Clamp(CurrTmDivTransDur, 0.0f, 1.0f)};

    // Bezier Curve
    float BezierCurve {FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f)};

    if (bUseBezierCurve) {

        if (bCinematicCameraSwitch)
            LerpedArmLength = FMath::Lerp(CameraArmLengthStart, CameraArmLengthEnd, BezierCurve);
        else
            LerpedArmLength = FMath::Lerp(CameraArmLengthEnd, CameraArmLengthStart, BezierCurve);

    } else {

        if (bCinematicCameraSwitch)
            LerpedArmLength = FMath::Lerp(CameraArmLengthStart, CameraArmLengthEnd, CurrTmDivTransDur);
        else
            LerpedArmLength = FMath::Lerp(CameraArmLengthEnd, CameraArmLengthStart, CurrTmDivTransDur);
    }

    // Configure o comprimento do braço da câmera
    CameraBoom->TargetArmLength = LerpedArmLength;

    // Verifica se a transição está concluída
    if (CurrentTime >= TransitionDuration) {

        if (bCinematicCameraSwitch)
            CinematicCameraOn();
        else
            CinematicCameraOff();

        bIsTransitioning = false;
    }
}

void AShooterCharacter::ToggleCinematicCamera() {

    return;
    bIsTransitioning       = !bIsTransitioning;
    bCinematicCameraSwitch = !bCinematicCameraSwitch;
    CurrentTime            = 0.0f; // Reinicia o tempo atua
}

void AShooterCharacter::Aim() {

    bAiming = true;

    GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void AShooterCharacter::StopAiming() {

    bAiming = false;

    if (!bCrouching) GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void AShooterCharacter::PickupAmmo(AAmmo *Ammo) {

    // check to see if AmmoMap contains Ammo's AmmoType
    if (AmmoMap.Find(Ammo->GetAmmoType())) {

        // Get amounts of ammo in our AmmoMap for Ammo's AmmoType
        int32 AmmoCount {AmmoMap [Ammo->GetAmmoType()]};

        AmmoCount += Ammo->GetItemCount();

        // Set the amounts of ammo in the map for this type
        AmmoMap [Ammo->GetAmmoType()] = AmmoCount;
    }

    if (EquippedWeapon->GetAmmoType() == Ammo->GetAmmoType()) {

        // Check if the gun is empty
        if (EquippedWeapon->GetAmmo() == 0) ReloadWeapon();
    }

    Ammo->Destroy();
}

void AShooterCharacter::AimingCameraZoom(float DeltaTime) {

    // Set current camera field of view
    if (bAiming) {

        CameraCurrentAimFOV
          = BezierCurve_Interp(CameraCurrentAimFOV, CameraZoomedAimFOV, DeltaTime, ZoomInterpSpeed, false);

        CinematicCameraOff();

    } else {

        // Interpolate to default FOV
        CameraCurrentAimFOV
          = FMath::FInterpTo(CameraCurrentAimFOV, CameraDefaultAimFOV, DeltaTime, ZoomInterpSpeed);

        CinematicCameraOn();
    }

    GetFollowCamera()->SetFieldOfView(CameraCurrentAimFOV);
}

float AShooterCharacter::BezierCurve_Interp(const float &CurrentFov, const float &TargetFov, float DeltaTime,
  float InterpSpeed, bool bReturnToOriginal) {

    auto BezierInterpolation
      = [](float Alpha) -> float { return (3 * Alpha * Alpha - 2 * Alpha * Alpha * Alpha); };

    float CurrentFOVCopy = CurrentFov;
    float TargetFOVCopy  = TargetFov;

    // Calcula o valor da curva de Bezier para controlar a interpolação
    float TargetValue = bReturnToOriginal ? CurrentFOVCopy : TargetFOVCopy;
    float BezierAlpha = BezierInterpolation(FMath::Clamp(CurrentFOVCopy / TargetValue, 0.0f, 1.0f));

    // Interpola usando o resultado da curva de Bezier nos valores copiados
    float InterpolatedFOV
      = FMath::FInterpTo(CurrentFOVCopy, TargetValue, DeltaTime, InterpSpeed) * BezierAlpha;

    // O valor de CameraCurrentFOV não é modificado

    return InterpolatedFOV;

    // Atualiza o valor de CameraCurrentFOV
    CameraCurrentAimFOV = InterpolatedFOV;

    return InterpolatedFOV;
}

void AShooterCharacter::CalculateCrosshairSpread(float DeltaTime) {

    Fvc2 WalkSpeedRange {0.f, 600.f};
    Fvc2 VelocityMultiplierRange {0.f, 1.f};
    Fvc  Velocity {GetVelocity()};

    Velocity.Z = 0.f;

    // Calculate crosshair velocity factor
    CrosshairVelocityFactor
      = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

    // Calculate crosshair in air factor
    if (GetCharacterMovement()->IsFalling()) { // is in air?

        // Spread the crosshairs slowly while in air
        CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 1.25f, DeltaTime, 10.f);

    } else { // Character is on the ground

        // Shrink the crosshairs rapidly while on the ground
        CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
    }

    // Calculate crosshair aim factor
    if (bAiming) { // is aiming?

        // Shrink crosshairs a small amount very quickly
        CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.f);


    } else { // Not aiming

        // Spread crosshairs rapidly
        CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
    }

    // True 0.05 seconds after firing
    if (bFiringBullet) {

        CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.f);

        Toggle_bIsFiringWeapon_On();

    } else {

        CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 60.f);

        Toggle_bIsFiringWeapon_Off();
    }

    CrosshairSpreadMultiplier
      = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
}

void AShooterCharacter::FireWeapon() {

    if ((EquippedWeapon == nullptr) || (CombatState != ECombatState::ECS_Unoccupied) || (!WeaponHasAmmo()))
        return;

    if (WeaponHasAmmo()) {

        PlayFireSound();
        SendBullet();
        PlayGunFireMontage();

        // Start Bullet Fire for Crosshairs
        StartCrosshairBulletFire();

        // Decrement Ammo by 1
        EquippedWeapon->DecrementAmmo();

        StartFireTimer();
    }
}

bool AShooterCharacter::GetBeanEndLocation(const FVector &MuzzleSocketLocation, FVector &OutBeanLocation) {

    // Check for Crosshair Trace Hit
    FHitResult CrosshairHitResult;
    bool       bCrosshairHit {TraceUnderCrosshairs(CrosshairHitResult, OutBeanLocation)};

    if (bCrosshairHit)
        // Tentative beam location - still need to trace from gun
        OutBeanLocation = CrosshairHitResult.Location;

    // Perform a second trace, this time from the barrel socket
    FHitResult    WeaponTraceHit;
    const FVector WeaponTraceStart {MuzzleSocketLocation};
    const FVector StartToEnd {OutBeanLocation - MuzzleSocketLocation};
    const FVector WeaponTraceEnd {MuzzleSocketLocation + StartToEnd * 1.25f};

    GetWorld()->LineTraceSingleByChannel(
      WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);

    // Object between barrel and BeanEndPoint?
    if (WeaponTraceHit.bBlockingHit) {

        OutBeanLocation = WeaponTraceHit.Location;

        return true;
    }
    return false;
}

void AShooterCharacter::Relative_ControllerRotationYaw(float DeltaTime) {

    // Obtém a velocidade atual do personagem
    float CurrentSpeed = GetVelocity().Size();

    // Define um limite de velocidade mínima para considerar que o personagem
    // está se movendo
    float MovementThreshold = 10.0f; // Ajuste conforme necessário

    // Verifica se o personagem está se movendo
    bool bIsMoving = CurrentSpeed > MovementThreshold;

    // Agora você pode usar bIsMoving conforme necessário
    if (bIsMoving && !bIsFiringWeapon) {

        bUseControllerRotationYaw = true;

        // O personagem está se movendo, você pode fazer algo aqui
    } else {

        bUseControllerRotationYaw = false;

        // O personagem não está se movendo, você pode fazer algo aqui
    }

    if ((bIsFiringWeapon && !bIsMoving && bDidFire) || bAiming) {

        // Obtém a rotação atual do controlador
        const FRotator ControllerRotation = Controller->GetControlRotation();

        // Armazene o yaw atual do controlador
        float CurrentYaw = ControllerRotation.Yaw;

        FVector MoveDirection = ControllerRotation.Vector();

        // Normaliza a direção para ter comprimento 1 (para manter uma
        // velocidade de movimento constante)
        MoveDirection.Normalize();

        // Ajuste a velocidade de movimento conforme necessário
        float MoveSpeed = 1.0f; // Ajuste conforme necessário

        // Calcule o vetor de movimento multiplicando a direção pela velocidade
        FVector MovementVector = MoveDirection * MoveSpeed;

        //&& CurrentYaw != PreviousYaw

        if (bAiming || FMath::Abs(CurrentYaw)) {

            bUseControllerRotationYaw = true;

            // Chame uma função para mover o personagem apenas quando o yaw for
            // maior que 40 graus
            if (GetCharacterMovement()) GetCharacterMovement()->AddForce(MovementVector);

            PreviousYaw = CurrentYaw;
        }
    }
}

void AShooterCharacter::SetupFollowCamera() {

    if (FollowCamera) {

        CameraDefaultAimFOV = GetFollowCamera()->FieldOfView;
        CameraCurrentAimFOV = CameraDefaultAimFOV;

    } else {

        PrintLogErr("AShooterCharacter::SetupFollowCamera(): Follow Camera is nullptr");
    }
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {

    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);

    /* Move Forward and Right */ {
        PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
    }

    /* Turn and LookUp with arrow keys */ {
        PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
        PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
    }

    /* Turn and LookUp with mouse */ {
        PlayerInputComponent->BindAxis("Turn", this, &AShooterCharacter::Turn);
        PlayerInputComponent->BindAxis("LookUp", this, &AShooterCharacter::LookUp);
    }

    PlayerInputComponent->BindAxis("MouseWheel", this, &AShooterCharacter::HandleMouseWheel);


    /* Jump */ {
        PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterCharacter::Jump);

        PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    }

    // Toggle Cinematic Camera
    PlayerInputComponent->BindAction(
      "ToggleCamera", IE_Pressed, this, &AShooterCharacter::ToggleCinematicCamera);

    /* Fire Weapon */ {
        PlayerInputComponent->BindAction(
          "FireButton", IE_Pressed, this, &AShooterCharacter::FireButtonPressed);

        PlayerInputComponent->BindAction(
          "FireButton", IE_Released, this, &AShooterCharacter::FireButtonReleased);
    }

    /* Aim Weapon */ {
        PlayerInputComponent->BindAction(
          "AimingButton", IE_Pressed, this, &AShooterCharacter::AimingButtonPressed);

        PlayerInputComponent->BindAction(
          "AimingButton", IE_Released, this, &AShooterCharacter::AimingButtonReleased);
    }

    /* Select */ {
        PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);

        PlayerInputComponent->BindAction(
          "Select", IE_Released, this, &AShooterCharacter::SelectButtonReleased);
    }

    // Reload Weapon
    PlayerInputComponent->BindAction(
      "ReloadButton", IE_Pressed, this, &AShooterCharacter::ReloadButtonPressed);

    // Crouch
    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterCharacter::CrouchButtonPressed);

    /* Letter keys */ {

        PlayerInputComponent->BindAction("Fkey", IE_Pressed, this, &AShooterCharacter::KEY_FkeyPressed);
    }

    /* Number keys */ {

        PlayerInputComponent->BindAction("1Key", IE_Pressed, this, &AShooterCharacter::KEY_1_OneKeyPressed);

        PlayerInputComponent->BindAction("2Key", IE_Pressed, this, &AShooterCharacter::KEY_2_TwoKeyPressed);

        PlayerInputComponent->BindAction("3Key", IE_Pressed, this, &AShooterCharacter::KEY_3_ThreeKeyPressed);

        PlayerInputComponent->BindAction("4Key", IE_Pressed, this, &AShooterCharacter::KEY_4_FourKeyPressed);

        PlayerInputComponent->BindAction("5Key", IE_Pressed, this, &AShooterCharacter::KEY_5_FiveKeyPressed);

        PlayerInputComponent->BindAction("6Key", IE_Pressed, this, &AShooterCharacter::KEY_6_SixKeyPressed);

        PlayerInputComponent->BindAction("7Key", IE_Pressed, this, &AShooterCharacter::KEY_7_SevenKeyPressed);

        PlayerInputComponent->BindAction("8Key", IE_Pressed, this, &AShooterCharacter::KEY_8_EightKeyPressed);

        PlayerInputComponent->BindAction("9Key", IE_Pressed, this, &AShooterCharacter::KEY_9_NineKeyPressed);

        PlayerInputComponent->BindAction("0Key", IE_Pressed, this, &AShooterCharacter::KEY_0_ZeroKeyPressed);
    }
}

void AShooterCharacter::DefaultConstructor_SetupMesh() {

    const static TCHAR *SkeletalMeshContainerPath {
      L"/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/"
      L"Belica/Meshes/Belica.Belica'"};

    SkeletalMeshContainer
      = MakeUnique<ConstructorHelpers::FObjectFinder<USkeletalMesh>>(SkeletalMeshContainerPath);

    PlayerMesh = GetMesh();

    if (PlayerMesh) {

        if (SkeletalMeshContainer->Succeeded()) {

            PlayerMesh->SetSkeletalMesh(SkeletalMeshContainer->Object);

            PlayerMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

            PlayerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
            PlayerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        } else {
            PrintLogErr("AShooterCharacter::DefaultConstructor_SetupMesh(): "
                        "SkeletalMeshContainer failed");
        }
    } else {
        PrintLogErr("AShooterCharacter::DefaultConstructor_SetupMesh(): PlayerMesh "
                    "is nullptr");
    }
}

void AShooterCharacter::DefaultConstructor_SetCombatCues() {

    if (FireSound == nullptr) {

        FireSound = CDSubObj<USoundCue>(L"FireSound");

        const static TCHAR *FireSoundCuePath {
          L"/Game/_Game/Assets/Sounds/GunShots/AR15_Shots/AR_Shot.AR_Shot"};

        auto const static M_FireSoundCue {ConstructorHelpers::FObjectFinder<USoundCue>(FireSoundCuePath)};

        if (M_FireSoundCue.Succeeded())
            FireSound = M_FireSoundCue.Object;
        else {
            PrintLogErr("AShooterCharacter::DefaultConstructor_SetCombatCues(): "
                        "M_FireSoundCue failed");
        }
    }

    if (MuzzleFlash == nullptr) {

        MuzzleFlash = CDSubObj<UParticleSystem>(L"MuzzleFlash");

        const static TCHAR *MuzzleFlashPath {L"/Script/Engine.ParticleSystem'/Game/_Game/Assets/FX/"
                                             L"P_BelicaMuzzle_SigleBrust.P_BelicaMuzzle_SigleBrust'"};

        auto const static M_MuzzleFlashParticle {
          ConstructorHelpers::FObjectFinder<UParticleSystem>(MuzzleFlashPath)};

        if (M_MuzzleFlashParticle.Succeeded())
            MuzzleFlash = M_MuzzleFlashParticle.Object;
        else {
            PrintLogErr("AShooterCharacter::DefaultConstructor_SetCombatCues(): "
                        "M_MuzzleFlashParticle failed");
        }
    }

    if (HipFireMontage == nullptr) {

        HipFireMontage = CDSubObj<UAnimMontage>(L"HipFireMontage");

        auto const M_Animation_HipFireMontage {ConstructorHelpers::FObjectFinder<UAnimMontage>(
          L"/Script/Engine.AnimMontage'/Game/_Game/Character/Animations/"
          L"HipFireMontage.HipFireMontage'")};

        if (M_Animation_HipFireMontage.Succeeded()) HipFireMontage = M_Animation_HipFireMontage.Object;
    }

    if (ImpactParticles == nullptr) {

        ImpactParticles = CDSubObj<UParticleSystem>(L"ImpactParticles");

        auto const M_ImpactParticle {ConstructorHelpers::FObjectFinder<UParticleSystem>(
          L"/Script/Engine.ParticleSystem'/Game/ParagonLtBelica/FX/Particles/"
          L"Belica/Abilities/Primary/FX/P_BelicaHitWorld.P_BelicaHitWorld'")};

        if (M_ImpactParticle.Succeeded()) ImpactParticles = M_ImpactParticle.Object;
    }

    if (BeamParticles == nullptr) {

        BeamParticles = CDSubObj<UParticleSystem>(L"BeamParticles");

        auto const M_BeamParticle {ConstructorHelpers::FObjectFinder<UParticleSystem>(
          L"/Script/Engine.ParticleSystem'/Game/_Game/Assets/FX/SmokeBean/"
          L"P_SmokeTrail_Faded.P_SmokeTrail_Faded'")};

        if (M_BeamParticle.Succeeded()) BeamParticles = M_BeamParticle.Object;
    }

    if (MuzzleFlash == nullptr) {

        MuzzleFlash = CDSubObj<UParticleSystem>(L"MuzzleFlash");

        auto const M_MuzzleFlashParticle {ConstructorHelpers::FObjectFinder<UParticleSystem>(
          L"/Script/Engine.ParticleSystem'/Game/_Game/Assets/FX/"
          L"P_BelicaMuzzle_SigleBrust.P_BelicaMuzzle_SigleBrust'")};

        if (M_MuzzleFlashParticle.Succeeded()) MuzzleFlash = M_MuzzleFlashParticle.Object;
    }

    if (ReloadMontage == nullptr) {

        ReloadMontage = CDSubObj<UAnimMontage>(L"ReloadMontage");

        auto const M_ReloadMontage {ConstructorHelpers::FObjectFinder<UAnimMontage>(
          L"/Script/Engine.AnimMontage'/Game/_Game/Character/Animations/"
          L"ReloadMontage.ReloadMontage'")};

        if (M_ReloadMontage.Succeeded()) ReloadMontage = M_ReloadMontage.Object;
    }
}

void AShooterCharacter::DefaultConstructor_SetCharacter() {

    // Don't rotate when the controller rotates. Let that just affect the
    // camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;

    AutoPossessPlayer = EAutoReceiveInput::Player0;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate              = FRotator(0.f, 540.f, 0.f); //... at this rotation rate
    GetCharacterMovement()->JumpZVelocity             = 600.f;
    GetCharacterMovement()->AirControl                = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed              = 1500.f;
    GetCharacterMovement()->MaxWalkSpeedCrouched      = 150.f;
    GetCharacterMovement()->MaxAcceleration           = 1020.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 50.f;
    GetCharacterMovement()->GroundFriction             = 1.5f;
}

void AShooterCharacter::DefaultConstructor_InterpComponents() {

    WeaponInterpComp = CDSubObj<USceneComponent>(L"Weapon Interpolation Component");
    WeaponInterpComp->SetupAttachment(GetFollowCamera());

    InterpComp1 = CDSubObj<USceneComponent>(L"Interpolation Component 1");
    InterpComp1->SetupAttachment(GetFollowCamera());

    InterpComp2 = CDSubObj<USceneComponent>(L"Interpolation Component 2");
    InterpComp2->SetupAttachment(GetFollowCamera());

    InterpComp3 = CDSubObj<USceneComponent>(L"Interpolation Component 3");
    InterpComp3->SetupAttachment(GetFollowCamera());

    InterpComp4 = CDSubObj<USceneComponent>(L"Interpolation Component 4");
    InterpComp4->SetupAttachment(GetFollowCamera());

    InterpComp5 = CDSubObj<USceneComponent>(L"Interpolation Component 5");
    InterpComp5->SetupAttachment(GetFollowCamera());

    InterpComp6 = CDSubObj<USceneComponent>(L"Interpolation Component 6");
    InterpComp6->SetupAttachment(GetFollowCamera());
}

void AShooterCharacter::DefaultConstructor_CameraBoom() {

    // Create a camera boom (pulls in towards the character if there is a
    // collision)
    CameraBoom = CDSubObj<USpringArmComponent>(L"Camera Boom");
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength         = 200.f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate at the arm base on the controller
}

void AShooterCharacter::DefaultConstructor_FollowCamera() {

    // Create a Follow Camera
    FollowCamera = CDSubObj<UCameraComponent>(L"FollowCamera");

    // attach camera to the end of boom
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AShooterCharacter::Toggle_bIsFiringWeapon_On() {

    bIsFiringWeapon = !bIsFiringWeapon;
    bDidFire        = !bDidFire;
}

void AShooterCharacter::Toggle_bIsFiringWeapon_Off() {

    bIsFiringWeapon = false;
    bDidFire        = false;
}

void AShooterCharacter::AimingButtonPressed() {

    bAimingButtonPressed = true;

    if (CombatState != ECombatState::ECS_Reloading) Aim();
}

void AShooterCharacter::AimingButtonReleased() {

    bAimingButtonPressed = false;
    StopAiming();
}

void AShooterCharacter::SetsLookRates() {

    if (bAiming) {

        BaseTurnRate   = AimingTurnRate;
        BaseLookUpRate = AimingLookUpRate;

    } else {

        BaseTurnRate   = HipTurnRate;
        BaseLookUpRate = HipLookUpRate;
    }
}

void AShooterCharacter::CinematicCameraOn() {

    CameraBoom->bEnableCameraLag         = true;
    CameraBoom->bEnableCameraRotationLag = true;
    CameraBoom->bUseCameraLagSubstepping = true;
    CameraBoom->CameraLagSpeed           = 6.f;
    CameraBoom->CameraRotationLagSpeed   = 10.f;
    CameraBoom->CameraLagMaxTimeStep     = 0.016667f;
}

void AShooterCharacter::CinematicCameraOff() {

    CameraBoom->bEnableCameraLag         = false;
    CameraBoom->bEnableCameraRotationLag = false;
    CameraBoom->bUseCameraLagSubstepping = false;
}

void AShooterCharacter::MoveForward(float Value) {

    if (Controller && Value != 0.0f) {

        // find which way is forward
        const FRotator Rotation {Controller->GetControlRotation()};
        const FRotator YawRotation {0, Rotation.Yaw, 0};

        const FVector Direction {FRotationMatrix {YawRotation}.GetUnitAxis(EAxis::X)};

        AddMovementInput(Direction, Value);
    }
}

void AShooterCharacter::MoveRight(float Value) {

    if (Controller && Value != 0.0f) {

        // find which way is right
        const FRotator Rotation {Controller->GetControlRotation()};
        const FRotator YawRotation {0, Rotation.Yaw, 0};

        const FVector Direction {FRotationMatrix {YawRotation}.GetUnitAxis(EAxis::Y)};

        AddMovementInput(Direction, Value);
    }
}

void AShooterCharacter::TurnAtRate(float Rate) {

    // calculate delta for this frame from the rate information
    AddControllerYawInput(
      Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); /* deg\sec multiply by sec\frame */
}

void AShooterCharacter::LookUpAtRate(float Rate) {
    AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value) {

    float TurnScaleFactor {};

    if (bAiming)
        TurnScaleFactor = MouseAimingTurnRate;
    else
        TurnScaleFactor = MouseHipTurnRate;

    AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value) {

    float LookUpScaleFactor {};

    if (bAiming)
        LookUpScaleFactor = MouseAimingLookUpRate;
    else
        LookUpScaleFactor = MouseHipLookUpRate;


    AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::StartCrosshairBulletFire() {

    bFiringBullet = true;

    GetWorldTimerManager().SetTimer(
      CrosshairShootTimer, this, &AShooterCharacter::FinishCrosshairBulletFire, ShootTimeDuration);
}

void AShooterCharacter::FinishCrosshairBulletFire() { bFiringBullet = false; }

void AShooterCharacter::FireButtonPressed() {

    bFireButtonPressed = true;
    FireWeapon();
}

void AShooterCharacter::FireButtonReleased() { bFireButtonPressed = false; }

void AShooterCharacter::StartFireTimer() {

    CombatState = ECombatState::ECS_FireTimerInProgress;

    if (bShouldFire) {

        GetWorldTimerManager().SetTimer(
          AutoFireTimer, this, &AShooterCharacter::AutoFireReset, AutomaticFireRate);
    }
}

void AShooterCharacter::AutoFireReset() {

    CombatState = ECombatState::ECS_Unoccupied;

    if (WeaponHasAmmo()) {

        if (bFireButtonPressed) FireWeapon();

    } else {

        ReloadWeapon();
    }
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult &OutHitResult, FVector &OutHitlocation) {

    // Get Viewport Size
    FVector2D ViewPortSize;
    if (GEngine && GEngine->GameViewport) GEngine->GameViewport->GetViewportSize(ViewPortSize);

    // Get Screen Space of Crosshair Location
    FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
    CrosshairLocation.Y -= CrosshairHeight;
    FVector CrosshairWorldPosition;
    FVector CrosshairWorldDirection;

    // Get World position and direction of Crosshair
    bool bScreenToWorld {
      UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
        CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection)};

    if (bScreenToWorld) {

        // Trace from Crosshair world location outward
        const FVector Start {CrosshairWorldPosition};
        const FVector End {Start + CrosshairWorldDirection * 50'000.f};
        OutHitlocation = End;

        // Trace outward from Crosshair world location
        if (GetWorld())
            GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

        if (OutHitResult.bBlockingHit) {

            OutHitlocation = OutHitResult.Location;
            return true;
        }
    }

    return false;
}

void AShooterCharacter::TraceForItems() {

    if (bShouldTraceForItems) {

        FHitResult ItemHitResult {};
        FVector    HitLocation {};
        TraceUnderCrosshairs(ItemHitResult, HitLocation);

        if (ItemHitResult.bBlockingHit) {

            // Cast to AItem to check if it's valid and if it has a widget
            TraceHitItem = Cast<AItem>(ItemHitResult.GetActor());

            // if hititem is in EquipInterping state set to nullptr
            if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping) {

                TraceHitItem = nullptr;
            }

            if (TraceHitItem && TraceHitItem->GetPickupWidget()) {

                // Show the pickup widget
                TraceHitItem->GetPickupWidget()->SetVisibility(true);

                TraceHitItem->EnableCustomDepth();
            }

            // We hit an item last frame
            if (TraceHitItemLastFrame) {

                if (TraceHitItem != TraceHitItemLastFrame) {

                    // We are hitting a deferent item this frame
                    // Or AItem is null
                    TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);

                    TraceHitItemLastFrame->DisableCustomDepth();
                }
            }
            // Store a reference to the HitItem for next frame
            TraceHitItemLastFrame = TraceHitItem;
        }

    } else if (TraceHitItemLastFrame) {

        // We are not hitting an item this frame
        // Hide the widget of the item we were hitting last frame
        TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);

        TraceHitItemLastFrame->DisableCustomDepth();
    }
}

AWeapon *AShooterCharacter::SpawnDefaultWeapon() {

    // Check the TSubclassOf variable
    if (DefaultWeaponClass)

        // Spawn the Weapon
        return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);

    return nullptr;
}

void AShooterCharacter::EquipWeapon(AWeapon *WeaponToEquip) {

    if (WeaponToEquip) {

        // Get the Hand Socket
        const USkeletalMeshSocket *HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));

        if (HandSocket) {

            // Attach the Weapon to the hand socket RightHandSocket
            HandSocket->AttachActor(WeaponToEquip, GetMesh());
        }

        if (EquippedWeapon == nullptr) {

            // -1 == no EquippedWeapon yet. No need to reverse the icon animation
            EquipItemDelegate.Broadcast(-1, WeaponToEquip->GetSlotIndex());

        } else {

            EquipItemDelegate.Broadcast(EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
        }

        // Set EquippedWeapon to the newly spawned Weapon
        EquippedWeapon = WeaponToEquip;
        EquippedWeapon->SetItemState(EItemState::EIS_Equipped);

    } else {
        ExitGameErr("AShooterCharacter::EquipWeapon(): WeaponToEquip is nullptr");
    }
}

void AShooterCharacter::DropWeapon() {

    if (EquippedWeapon) {

        FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

        EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

        EquippedWeapon->SetItemState(EItemState::EIS_Falling);
        EquippedWeapon->ThrowWeapon();
    }
}

void AShooterCharacter::SwapWeapon(AWeapon *WeaponToSwap) {

    // Makes sure if the inventory can handle the SlotIndex of the EquippedWeapon
    if ((Inventory.Num() - 1) >= (EquippedWeapon->GetSlotIndex())) {

        // If the inventory can handle the SlotIndex of the EquippedWeapon
        // then we can add the WeaponToSwap to the Inventory
        Inventory [EquippedWeapon->GetSlotIndex()] = WeaponToSwap;

        WeaponToSwap->SetSlotIndex(EquippedWeapon->GetSlotIndex());
    }

    DropWeapon();
    EquipWeapon(WeaponToSwap);
    TraceHitItem          = nullptr;
    TraceHitItemLastFrame = nullptr;
}

void AShooterCharacter::InitializeAmmoMap() {

    AmmoMap.Add(EAmmoType::EAT_9mm, Starting_9mm_Ammo);
    AmmoMap.Add(EAmmoType::EAT_AR, Starting_AR_Ammo);
}

bool AShooterCharacter::WeaponHasAmmo() {

    if (EquippedWeapon == nullptr) return false;

    return EquippedWeapon->GetAmmo() > 0;
}

void AShooterCharacter::PlayFireSound() {

    if (FireSound) UGameplayStatics::PlaySound2D(this, FireSound);
}

void AShooterCharacter::SendBullet() {

    const USkeletalMeshSocket *BarrelSocket {EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket")};

    if (BarrelSocket) {

        const FTransform SocketTransform {BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh())};

        if (MuzzleFlash) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);

        FVector BeamEnd;

        // Get Beam End Location
        bool bBeanEnd {GetBeanEndLocation(SocketTransform.GetLocation(), BeamEnd)};

        if (bBeanEnd) {

            if (ImpactParticles)
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamEnd);

            UParticleSystemComponent *Beam {
              UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform)};

            if (Beam) Beam->SetVectorParameter(FName("Target"), BeamEnd);
        }
    } else {
        ExitGameErr("AShooterCharacter::SendBullet(): BarrelSocket is nullptr");
    }
}

void AShooterCharacter::PlayGunFireMontage() {

    UAnimInstance *AnimInstance {GetMesh()->GetAnimInstance()};

    if (AnimInstance && HipFireMontage) {

        AnimInstance->Montage_Play(HipFireMontage);
        AnimInstance->Montage_JumpToSection(FName("StartFire"));
    }
}

void AShooterCharacter::ReloadButtonPressed() { ReloadWeapon(); }

void AShooterCharacter::ReloadWeapon() {

    if ((CombatState != ECombatState::ECS_Unoccupied) || (EquippedWeapon == nullptr) || (!CarryingAmmo())
        || (EquippedWeapon->ClipIsFull()))
        return;

    // Do we have ammo of the correct type?
    if ((CarryingAmmo()) && (!EquippedWeapon->ClipIsFull())) {

        if (bAiming) StopAiming();

        CombatState = ECombatState::ECS_Reloading;
        UAnimInstance *AnimInstance {GetMesh()->GetAnimInstance()};

        if (ReloadMontage && AnimInstance) {

            AnimInstance->Montage_Play(ReloadMontage);

            AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
        }
    }
}

void AShooterCharacter::FinishReloading() {

    // Update the combat state
    CombatState = ECombatState::ECS_Unoccupied;

    if (bAimingButtonPressed) Aim();

    if (EquippedWeapon == nullptr) return;

    const auto AmmoType {EquippedWeapon->GetAmmoType()};

    // Update the ammo map
    if (AmmoMap.Contains(AmmoType)) {

        // Amount of ammo the character is carrying of the equipped weapons type
        int32 CarriedAmmo {AmmoMap [AmmoType]};

        // Space left in the magazine of the equipped weapon
        const int32 MagEmptySpace {EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo()};

        if (MagEmptySpace > CarriedAmmo) {

            // Reload the Magazine with all the ammo we are caring
            EquippedWeapon->ReloadAmmo(CarriedAmmo);
            CarriedAmmo = 0;

            AmmoMap.Add(AmmoType, CarriedAmmo);

        } else {

            // Fill the magazine
            EquippedWeapon->ReloadAmmo(MagEmptySpace);
            CarriedAmmo -= MagEmptySpace;

            AmmoMap.Add(AmmoType, CarriedAmmo);
        }
    }
}

void AShooterCharacter::FinishEquipping() {

    // Update the combat state
    CombatState = ECombatState::ECS_Unoccupied;
}

bool AShooterCharacter::CarryingAmmo() {

    if (EquippedWeapon == nullptr) return false;

    EAmmoType AmmoType {EquippedWeapon->GetAmmoType()};

    if (AmmoMap.Contains(AmmoType)) {
        return AmmoMap [AmmoType] > 0;
    }
    return false;
}

void AShooterCharacter::GrabClip() {

    if ((EquippedWeapon == nullptr) || (HandSceneComponent == nullptr)) return;

    // Index for the clip bone on the EquippedWeapon
    int32 ClipBoneIndex {EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName())};

    // Store the transform of the clip bone
    ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);

    HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(L"Hand_L"));

    HandSceneComponent->SetWorldTransform(ClipTransform);

    EquippedWeapon->SetMovingClip(true);
}

void AShooterCharacter::ReleaseClip() {

    if (EquippedWeapon == nullptr) return;
    EquippedWeapon->SetMovingClip(false);
}

void AShooterCharacter::CrouchButtonPressed() {

    if (!GetCharacterMovement()->IsFalling()) bCrouching = !bCrouching;

    if (bCrouching) {
        GetCharacterMovement()->MaxWalkSpeed   = CrouchMovementSpeed;
        GetCharacterMovement()->GroundFriction = CrouchingGroundFriction;
    } else {
        GetCharacterMovement()->MaxWalkSpeed   = BaseMovementSpeed;
        GetCharacterMovement()->GroundFriction = BaseGroundFriction;
    }
}

void AShooterCharacter::Jump() {

    if (bCrouching) {

        bCrouching                           = false;
        GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

    } else
        ACharacter::Jump();
}

void AShooterCharacter::SelectButtonPressed() {

    if (CombatState != ECombatState::ECS_Unoccupied) return;

    if (TraceHitItem) {

        TraceHitItem->StartItemCurve(this);
        TraceHitItem = nullptr;
    }
}

void AShooterCharacter::SelectButtonReleased() {}

float AShooterCharacter::GetCrosshairHeight() const { return CrosshairHeight; }

float AShooterCharacter::GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

void AShooterCharacter::IncrementOverlappedItemCount(int8 Amount) {

    if (OverlappedItemCount + Amount <= 0) {

        OverlappedItemCount  = 0;
        bShouldTraceForItems = false;

    } else {

        OverlappedItemCount  += Amount;
        bShouldTraceForItems  = true;
    }
}

FVector AShooterCharacter::GetCameraInterpLocation() {

    const FVector CameraWorldLocation {FollowCamera->GetComponentLocation()};
    const FVector CameraForward {FollowCamera->GetForwardVector()};

    return CameraWorldLocation + CameraForward * CameraInterpDistance
           + FVector(0.f, 0.f, CameraInterpElevation);
}

void AShooterCharacter::GetPickupItem(AItem *Item) {

    Item->PlayEquipSound();

    auto Weapon = Cast<AWeapon>(Item);
    if (Weapon) {

        // If Number of elements in array is less than the inventory capacity
        // add the weapon
        if (Inventory.Num() < InventoryCapacity) {

            Weapon->SetSlotIndex(Inventory.Num());
            Inventory.Add(Weapon);
            Weapon->SetItemState(EItemState::EIS_PickedUp);

        } else { // Inventory is full! Swap with EquippedWeapon

            SwapWeapon(Weapon);
        }
    }

    auto Ammo = Cast<AAmmo>(Item);
    if (Ammo) {
        PickupAmmo(Ammo);
    }
}

void AShooterCharacter::InitializeInterpLocations() {

    FInterpLocation WeaponLocation {WeaponInterpComp, 0};
    InterpLocations.Add(WeaponLocation);

    FInterpLocation InterpLoc1 {InterpComp1, 0};
    InterpLocations.Add(InterpLoc1);

    FInterpLocation InterpLoc2 {InterpComp2, 0};
    InterpLocations.Add(InterpLoc2);

    FInterpLocation InterpLoc3 {InterpComp3, 0};
    InterpLocations.Add(InterpLoc3);

    FInterpLocation InterpLoc4 {InterpComp4, 0};
    InterpLocations.Add(InterpLoc4);

    FInterpLocation InterpLoc5 {InterpComp5, 0};
    InterpLocations.Add(InterpLoc5);

    FInterpLocation InterpLoc6 {InterpComp6, 0};
    InterpLocations.Add(InterpLoc6);
}

inline void AShooterCharacter::ResetPickupSoundTimer() { bShouldPlayPickupSound = true; }

inline void AShooterCharacter::ResetEquipSoundTimer() { bShouldPlayEquipSound = true; }

void AShooterCharacter::ExchangeInventoryItens(int32 CurrentItemindex, int32 NewItemIndex) {

    if ((CurrentItemindex == NewItemIndex) ||
        (NewItemIndex >= Inventory.Num())  || 
        (CombatState != ECombatState::ECS_Unoccupied)) return;

    AWeapon *OldEquippedWeapon {EquippedWeapon};
    AWeapon *NewWeapon {Cast<AWeapon>(Inventory [NewItemIndex])};

    EquipWeapon(NewWeapon);

    OldEquippedWeapon->SetItemState(EItemState::EIS_PickedUp);
    NewWeapon->SetItemState(EItemState::EIS_Equipped);

    CombatState = ECombatState::ECS_Equipping;

    UAnimInstance *AnimInstance {nullptr};
    AnimInstance = GetMesh()->GetAnimInstance();

    if (AnimInstance) {
        if (EquipMontage){
            if (AnimInstance && EquipMontage) {

                AnimInstance->Montage_Play(EquipMontage, 1.0f);
                AnimInstance->Montage_JumpToSection(FName("Equip"));
            }
        } else {
            ExitPrintErr("AShooterCharacter::ExchangeInventoryItens(): EquipMontage is nullptr");
        }
    } else {
        ExitPrintErr("AShooterCharacter::ExchangeInventoryItens(): AnimInstance is nullptr");
    }
}

void AShooterCharacter::StartPickupSoundTimer() {

    bShouldPlayPickupSound = false;

    GetWorldTimerManager().SetTimer(
      PickupSoundTimer, this, &AShooterCharacter::ResetPickupSoundTimer, PickupSoundResetTime);
}

void AShooterCharacter::StartEquipSoundTimer() {

    bShouldPlayEquipSound = false;

    GetWorldTimerManager().SetTimer(
      EquipSoundTimer, this, &AShooterCharacter::ResetEquipSoundTimer, EquipSoundResetTime);
}

int32 AShooterCharacter::GetInterpLocationIndex() {

    int32 LowestIndex {1};
    int32 LowestCount {2'147'483'647}; /*two billion, one hundred forty -
      seven  hundred eighty-three thousand, six hundred forty-seven.*/


    for (int32 i = 1; i < InterpLocations.Num(); i++) {

        if (InterpLocations [i].ItemCount < LowestCount) {

            LowestIndex = i;
            LowestCount = InterpLocations [i].ItemCount;
        }
    }

    return LowestIndex;
}

FInterpLocation AShooterCharacter::GetInterpLocation(int32 Index) {

    if (Index <= InterpLocations.Num()) {
        return InterpLocations [Index];
    }

    return FInterpLocation();
}

void AShooterCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount) {

    if (Amount < -1 || Amount > 1) return;

    if (InterpLocations.Num() >= Index) {

        InterpLocations [Index].ItemCount += Amount;
    }
}

void AShooterCharacter::HandleMouseWheel(float Value) {

    if (Value != 0.0f) {
        if (Value > 0.0f) {

            ScrollUp();

        } else {

            ScrollDown();
        }
    }
}

void AShooterCharacter::DebugSlotsItens() {

    UpdateSlotsItens();

    if (GEngine) {

        GEngine->AddOnScreenDebugMessage(
          1, -1.f, FColor::Red, FString::Printf(L"Current Slot Index: %d", CurrentSlotIndex));
        GEngine->AddOnScreenDebugMessage(
          2, -1.f, FColor::Blue, FString::Printf(L"Inventory Count: %d", InventoryCount));
    }
}

void AShooterCharacter::UpdateSlotsItens() {

    CurrentSlotIndex = EquippedWeapon->GetSlotIndex();
    InventoryCount   = Inventory.Num();
}

void AShooterCharacter::ScrollUp() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (InventoryCount == 1)
            return;

        // Go to the first slot index
        else if ((CurrentSlotIndex >= EquippedWeapon->GetMaxSlotNumber())
                 || ((CurrentSlotIndex + 1) == InventoryCount))
            ExchangeInventoryItens(CurrentSlotIndex, 0);

        // advance the slot index
        else if ((InventoryCount - 1) > CurrentSlotIndex)
            ExchangeInventoryItens(CurrentSlotIndex, (CurrentSlotIndex + 1));

    } else {
        ExitPrintErr("AShooterCharacter::ScrollUp(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::ScrollDown() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (InventoryCount == 1)
            return;

        // Go to the last slot index
        else if (CurrentSlotIndex <= 0)
            return ExchangeInventoryItens(CurrentSlotIndex, (InventoryCount - 1));

        // Go back the slot index
        else if ((InventoryCount - 1) >= CurrentSlotIndex)
            return ExchangeInventoryItens(CurrentSlotIndex, (CurrentSlotIndex - 1));

    } else {
        ExitPrintErr("AShooterCharacter::ScrollDown(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_FkeyPressed() { KeyMethodFKey(); }
void AShooterCharacter::KeyMethodFKey() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 0) return;

       return ExchangeInventoryItens(CurrentSlotIndex, 0);

    } else {

        ExitPrintErr("AShooterCharacter::KEY_FkeyPressed(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_1_OneKeyPressed() { KeyMethod1Key(); }
void AShooterCharacter::KeyMethod1Key() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 1) return;

        return ExchangeInventoryItens(CurrentSlotIndex, 1);

    } else {

        ExitPrintErr("AShooterCharacter::KeyMethod1Key(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_2_TwoKeyPressed() { KeyMethod2Key(); }
void AShooterCharacter::KeyMethod2Key() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 2) return;

       return ExchangeInventoryItens(CurrentSlotIndex, 2);

    } else {

        ExitPrintErr("AShooterCharacter::KeyMethod2Key(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_3_ThreeKeyPressed() { KeyMethod3Key(); }
void AShooterCharacter::KeyMethod3Key() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 3) return;

        return ExchangeInventoryItens(CurrentSlotIndex, 3);

    } else {

        ExitPrintErr("AShooterCharacter::KeyMethod3Key(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_4_FourKeyPressed() { KeyMethod4Key(); }
void AShooterCharacter::KeyMethod4Key() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 4) return;

        return ExchangeInventoryItens(CurrentSlotIndex, 4);

    } else {

        ExitPrintErr("AShooterCharacter::KeyMethod4Key(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_5_FiveKeyPressed() { KeyMethod5Key(); }
void AShooterCharacter::KeyMethod5Key() {

    if (EquippedWeapon) {

        UpdateSlotsItens();

        if (CurrentSlotIndex == 5) return;

        return ExchangeInventoryItens(CurrentSlotIndex, 5);

    } else {

        ExitPrintErr("AShooterCharacter::KeyMethod5Key(): EquippedWeapon is nullptr");
    }
}

void AShooterCharacter::KEY_6_SixKeyPressed() { KeyMethod6Key(); }
void AShooterCharacter::KeyMethod6Key() { return; }

void AShooterCharacter::KEY_7_SevenKeyPressed() { KeyMethod7Key(); }
void AShooterCharacter::KeyMethod7Key() { return; }

void AShooterCharacter::KEY_8_EightKeyPressed() { KeyMethod8Key(); }
void AShooterCharacter::KeyMethod8Key() { return; }

void AShooterCharacter::KEY_9_NineKeyPressed() { KeyMethod9Key(); }
void AShooterCharacter::KeyMethod9Key() { return; }

void AShooterCharacter::KEY_0_ZeroKeyPressed() { KeyMethod0Key(); }
void AShooterCharacter::KeyMethod0Key() { return; }
