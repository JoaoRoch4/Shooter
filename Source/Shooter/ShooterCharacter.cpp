/*****************************************************************
 * \file   ShooterCharacter.cpp
 * \brief  Main Character Class
 * Code
 * \author João Rocha
 * \date   December 2023 -
 * January 2024
 *********************************************************************/

#include "ShooterCharacter.h"

#include "Ammo.h"
#include "AmmoType.h"
#include "Custom.h"
#include "Item.h"
#include "MovingDirection.h"
#include "ShooterCharacterCamera.h"
#include "Weapon.h"
#include "WeaponType.h"

#include "Containers/UnrealString.h.inl"
#include <Animation/AnimMontage.h>
#include <Animation\AnimInstance.h>
#include <Camera\CameraComponent.h>
#include <Chaos/ChaosEngineInterface.h>
#include <CollisionQueryParams.h>
#include <Components/InputComponent.h>
#include <Components/SceneComponent.h>
#include <Components\CapsuleComponent.h>
#include <Components\SkeletalMeshComponent.h>
#include <Containers/Array.h>
#include <Containers/Map.h>
#include <Engine/Engine.h>
#include <Engine/EngineBaseTypes.h>
#include <Engine/EngineTypes.h>
#include <Engine/HitResult.h>
#include <Engine/TimerHandle.h>
#include <Engine\SkeletalMesh.h>
#include <Engine\SkeletalMeshSocket.h>
#include <Engine\World.h>
#include <GameFramework/Character.h>
#include <GameFramework\CharacterMovementComponent.h>
#include <GameFramework\Pawn.h>
#include <GameFramework\SpringArmComponent.h>
#include <GenericPlatform/GenericPlatformMath.h>
#include <HAL/Platform.h>
#include <Kismet\GameplayStatics.h>
#include <Math/Axis.h>
#include <Math/Color.h>
#include <Math/MathFwd.h>
#include <Math/RotationMatrix.h>
#include <Math/UnrealMathUtility.h>
#include <Misc/AssertionMacros.h>
#include <Particles\ParticleSystem.h>
#include <Particles\ParticleSystemComponent.h>
#include <PhysicalMaterials/PhysicalMaterial.h>
#include <Templates/Casts.h>
#include <Templates/UniquePtr.h>
#include <UObject/NameTypes.h>
#include <UObject\ConstructorHelpers.h>

AShooterCharacter::AShooterCharacter()
 : PlayerMesh(nullptr)
 , SkeletalMeshContainer(nullptr)
 , CameraBoom(nullptr)
 , FollowCamera(nullptr)
 , CustomCamera(nullptr)
 , bUseCustomCamera(false)
 , bIsTransitioning(true)
 , bDKey_Pressed(false)
 , bDKey_Released(true)
 , bSKey_Pressed(false)
 , bSKey_Released(true)
 , bWKey_Pressed(false)
 , bWKey_Released(true)
 , bAKey_Pressed(false)
 , bAKey_Released(true)
 , bCinematicCameraSwitch(true)
 , bUseBezierCurve(true)
 , CameraArmLengthStart(200.f)
 , CameraArmLengthEnd(300.f)
 , TransitionDuration(0.2f)
 , PreviousYaw(NULL)
 , LeapedArmLength(NULL)
 , CurrentTime(NULL)
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
 , ImpactParticles(nullptr)
 , BeamParticles(nullptr)
 , HipFireMontage(nullptr)
 , EquipMontage(nullptr)
 , bShowCustomDebugMessages(false)
 , bAiming(false)
 , CameraDefaultAimFOV(NULL)
 , CameraZoomedAimFOV(40.f)
 , CameraCurrentAimFOV(0.f)
 , ZoomInterpSpeed(20.f)
 , CrosshairHeight(50.f)
 , CrosshairSpreadMultiplier(NULL)
 , CrosshairVelocityFactor(NULL)
 , CrosshairInAirFactor(NULL)
 , CrosshairAimFactor(NULL)
 , CrosshairShootingFactor(0.f)
 , ShootTimeDuration(0.05f)
 , bFiringBullet(false)
 , CrosshairShootTimer(FTimerHandle())
 , bFireButtonPressed(false)
 , bShouldFire(true)
 , AutoFireTimer(FTimerHandle())
 , bShouldTraceForItems(false)
 , OverlappedItemCount(NULL)
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
 , CurrentCapsuleHalfHeight(NULL)
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
 , CurrentSlotIndex(NULL)
 , InventoryCount(NULL)
 , ExchangeInventoryItensTimer(FTimerHandle())
 , bExchangeInventoryItensEnabled(true)
 , ExchangeInventoryItensTime(0.15f)
 , bDebugSlotMessages(false)
 , HighlightedSlot(-1)
 , GlobalDeltaTime(NULL)
 , BulletHoleDecal(nullptr)
 , BulletHoleDecalMat(nullptr)
 , bDisableCameraLagWhenMoving(false)
 , MovingDirection(EMovingDirection::EMD_None)
 , bShowEMovingDirection(true)
 , OffsetIdle(FVector(120.f, 60.f, 0.f))
 , CameraLagMaxDistance_Idle(100.0)
 , OriginalCameraSocketOffset(FVector::ZeroVector)
 , OriginalCameraLagSpeed(NULL)
 , OriginalCameraLagMaxDistance(NULL)
 , CustomCameraSocketOffset(FVector::ZeroVector)
 , CustomCameraLagSpeed(NULL)
 , CustomCameraLagMaxDistance(NULL)
 , OffsetFroward(FVector(-40.f, 0.f, 60.f))
 , CameraLagMaxDistance_Froward(200.0)
 , Froward_InterpTime(5.f)
 , OffsetBackwards(FVector(-140.f, 0.f, 0.f))
 , CameraLagMaxDistance_Backwards(100.0)
 , Backwards_InterpTime(5.f)
 , OffsetRight(FVector(0.f, 180.f, 40.f))
 , CameraLagMaxDistance_Right(100.0)
 , Right_InterpTime(5.f)
 , OffsetLeft(FVector(-40.f, 40.f, 40.f))
 , CameraLagMaxDistance_Left(100.0)
 , Left_InterpTime(5.f)
 , OffsetForwardRight(FVector(-40.f, 180.f, 60.f))
 , CameraLagMaxDistance_ForwardRight(100.f)
 , ForwardRight_InterpTime(5.f)
 , OffsetForwardLeft(FVector(-40.f, 60.f, 60.f))
 , CameraLagMaxDistance_ForwardLeft(100.0)
 , ForwardLeft_InterpTime(5.f)
 , OffsetBackwardRight(FVector(-140.f, 180.f, 60.f))
 , CameraLagMaxDistance_BackwardRight(NULL)
 , BackwardRight_InterpTime(5.f)
 , OffsetBackwardLeft(FVector(-140.f, 40.f, 60.f))
 , CameraLagMaxDistance_BackwardLeft(NULL)
 , BackwardLeft_InterpTime(5.f)
 , OffsetAim(FVector(0.f, 0.f, 60.f))
 , CameraLagMaxDistance_Aim(30.0)
 , Aim_interpTime(5.f)
 , OffsetForwardAim(FVector(0.f, 0.f, 60.f))
 , CameraLagMaxDistance_ForwardAim(30.0)
 , ForwardAim_InterpTime(5.f)
 , OffsetBackwardAim(FVector(0.f, 0.f, 60.f))
 , CameraLagMaxDistance_BackwardAim(30.0)
 , BackwardAim_InterpTime(5.f)
 , OffsetRightAim(FVector(0.f, 180.f, 60.f))
 , CameraLagMaxDistance_RightAim(30.0)
 , RightAim_InterpTime(5.f)
 , OffsetLeftAim(FVector(0.f, 40.f, 40.f))
 , CameraLagMaxDistance_LeftAim(30.0)
 , LeftAim_InterpTime(5.f)
 , OffsetForwardRightAim(FVector(0.f, 180.f, 60.f))
 , CameraLagMaxDistance_ForwardRightAim(30.0)
 , ForwardRightAim_InterpTime(5.f)
 , OffsetForwardLeftAim(FVector(0.f, 60.f, 60.f))
 , CameraLagMaxDistance_ForwardLeftAim(30.0)
 , ForwardLeftAim_InterpTime(5.f)
 , OffsetBackwardRightAim(FVector(0.f, 180.f, 60.f))
 , CameraLagMaxDistance_BackwardRightAim(30.0)
 , BackwardRightAim_InterpTime(5.f)
 , OffsetBackwardLeftAim(FVector(0.f, 40.f, 60.f))
 , CameraLagMaxDistance_BackwardLeftAim(30.0)
 , BackwardLeftAim_InterpTime(5.f)
 , OffsetJump(FVector(-100.f, 50.f, 90.f))
 , CameraLagMaxDistance_Jump(100.0)
 , JumpInterpTime(5.f)
 , bIsJumping(false)
 , OffsetForwardCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_ForwardCrouch(100.0)
 , ForwardCrouchInterpTime(5.f)
 , OffsetBackwardCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardCrouch(100.0)
 , BackwardCrouch_InterpTime(5.f)
 , OffsetRightCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_RightCrouch(100.0)
 , RightCrouch_InterpTime(5.f)
 , OffsetLeftCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_LeftCrouch(100.0)
 , LeftCrouch_InterpTime(5.f)
 , OffsetForwardRightCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_ForwardRightCrouch(100.0)
 , ForwardRightCrouch_InterpTime(5.f)
 , OffsetForwardLeftCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_ForwardLeftCrouch(100.0)
 , ForwardLeftCrouch_InterpTime(5.f)
 , OffsetBackwardRightCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardRightCrouch(100.0)
 , BackwardRightCrouch_InterpTime(5.f)
 , OffsetBackwardLeftCrouch(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardLeftCrouch(100.0)
 , BackwardLeftCrouch_InterpTime(5.f)
 , OffsetCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_CrouchAim(100.0)
 , ForwardCrouchAim_InterpTime(5.f)
 , OffsetBackwardCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardCrouchAim(100.f)
 , BackwardCrouchAim_InterpTime(5.f)
 , OffsetRightCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_RightCrouchAim(100.0)
 , RightCrouchAim_InterpTime(5.f)
 , OffsetLeftCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_LeftCrouchAim(100.0)
 , LeftCrouchAim_InterpTime(5.f)
 , OffsetForwardRightCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_ForwardRightCrouchAim(100.0)
 , ForwardRightCrouchAim_InterpTime(5.f)
 , OffsetForwardLeftCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_ForwardLeftCrouchAim(100.0)
 , ForwardLeftCrouchAim_InterpTime(5.f)
 , OffsetBackwardRightCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardRightCrouchAim(100.0)
 , BackwardRightCrouchAim_InterpTime(5.f)
 , OffsetBackwardLeftCrouchAim(FVector(0.f, 0.f, 0.f))
 , CameraLagMaxDistance_BackwardLeftCrouchAim(100.0)
 , BackwardLeftCrouchAim_InterpTime(5.f)
 , bNotMoving(false) {

	PrimaryActorTick.bCanEverTick = true;

	DefaultConstructor_SetupMesh();
	DefaultConstructor_SetCharacter();
	DefaultConstructor_CameraBoom();
	DefaultConstructor_FollowCamera();
	DefaultConstructor_CustomCamera();
	DefaultConstructor_SetCombatCues();

	// Create Hand Scene Component
	HandSceneComponent					 = CDSubObj<USceneComponent>(L"HandSceneComp");

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	DefaultConstructor_InterpComponents();

	AdjustVectors();
}

void AShooterCharacter::BeginPlay() {

	Super::BeginPlay();
	SetupFollowCamera();
	EquipWeapon();
	InitializeAmmoMap();
	InitializeInterpLocations();
	UpdateSlotsItems();
	GetOriginalCameraLagOffset();
	AdjustVectors();
	SetMovingDirection();
}

void AShooterCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	GlobalDeltaTime = DeltaTime;

	if (bIsTransitioning) StartCameraLerp(DeltaTime);

	// Relative_ControllerRotationYaw(DeltaTime);

	AimingCameraZoom(DeltaTime);
	SetsLookRates();
	CalculateCrosshairSpread(DeltaTime);

	if (bShouldTraceForItems || TraceHitItemLastFrame) TraceForItems();

	// Interpolate capsule half height based on crouching/standing
	InterpCapsuleHalfHeight(DeltaTime);

	if (bDebugSlotMessages) DebugSlotsItems();

	// DisableCameraLagWhenMovingRight(DeltaTime);

	if (!bDisableCameraLagWhenMoving) SetMovingDirectionActions(DeltaTime);
}

void AShooterCharacter::InterpCapsuleHalfHeight(float DeltaTime) {

	const float		TargetCapsuleHalfHeight = {bCrouching ?
												  CrouchingCapsuleHalfHeight :
												  StandingCapsuleHalfHeight};

	const float		GetScaledCapsule		= {GetCapsuleComponent()->GetScaledCapsuleHalfHeight()};
	constexpr float InterpSpeed				= {20.f};
	const float		InterpHalfHeight		= {FMath::FInterpTo(GetScaledCapsule, TargetCapsuleHalfHeight, DeltaTime, InterpSpeed)};

	// Negative value if crouching, positive value standing
	const float		DeltaCapsuleHalfHeight	= {InterpHalfHeight - GetScaledCapsule};

	// Offset mesh to compensate for capsule height change
	const FVector	MeshOffset {0.f, 0.f, -DeltaCapsuleHalfHeight};
	GetMesh()->AddLocalOffset(MeshOffset);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpHalfHeight);
}

void AShooterCharacter::StartCameraLerp(float &DeltaTime) {

	CurrentTime += DeltaTime;

	// get CurrentTime / TransitionDuration
	const float CurrTmDivTransDur = {CurrentTime / TransitionDuration};

	// Clamps CurrentTime / TransitionDuration
	const float Alpha			  = {FMath::Clamp(CurrTmDivTransDur, 0.0f, 1.0f)};

	// Bezier Curve
	const float BezierCurve		  = {FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f)};

	LeapedArmLength =
	   ((bUseBezierCurve) ?
			 ((bCinematicCameraSwitch) ?
				   FMath::Lerp(CameraArmLengthStart, CameraArmLengthEnd, BezierCurve) :
				   FMath::Lerp(CameraArmLengthEnd, CameraArmLengthStart, BezierCurve)) :
			 ((bCinematicCameraSwitch) ?
				   FMath::Lerp(CameraArmLengthStart, CameraArmLengthEnd, CurrTmDivTransDur) :
				   FMath::Lerp(CameraArmLengthEnd, CameraArmLengthStart, CurrTmDivTransDur)));

	// Configures the length of the camera arm
	CameraBoom->TargetArmLength = LeapedArmLength;

	// Verifies if the transition is finished
	if (CurrentTime >= TransitionDuration) {

		(bCinematicCameraSwitch) ? CinematicCameraOn() : CinematicCameraOff();

		bIsTransitioning = false;
	}
}

void AShooterCharacter::ToggleCinematicCamera() {

	return;
	/*
	bIsTransitioning       = !bIsTransitioning;
	bCinematicCameraSwitch = !bCinematicCameraSwitch;
	CurrentTime            = 0.0f; // Reinicia o tempo atua
	*/
}

void AShooterCharacter::Aim() {

	bAiming								 = true;

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	SetMovingDirection();
}

void AShooterCharacter::StopAiming() {

	bAiming = false;

	if (!bCrouching) GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	SetMovingDirection();
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

		CameraCurrentAimFOV = BezierCurve_Interp(
		   CameraCurrentAimFOV, CameraZoomedAimFOV, DeltaTime, ZoomInterpSpeed, false);

		CinematicCameraOff();

	} else {

		// Interpolate to default FOV
		CameraCurrentAimFOV = FMath::FInterpTo(CameraCurrentAimFOV, CameraDefaultAimFOV,
		   DeltaTime, ZoomInterpSpeed);

		CinematicCameraOn();
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentAimFOV);
}

float AShooterCharacter::BezierCurve_Interp(const float &CurrentFov, const float &TargetFov,
   float DeltaTime, float InterpSpeed, bool bReturnToOriginal) {

	auto BezierInterpolation = [&](const float &Alpha) -> const float {
		return (3 * Alpha * Alpha - 2 * Alpha * Alpha * Alpha);
	};

	const float CurrentFOVCopy	= {CurrentFov};
	const float TargetFOVCopy	= {TargetFov};
	const float TargetValue		= {bReturnToOriginal ? CurrentFOVCopy : TargetFOVCopy};
	const float BezierAlpha		= {BezierInterpolation(FMath::Clamp(CurrentFOVCopy / TargetValue,
		   0.0f, 1.0f))};

	// Interpola usando o resultado da curva de Bezier nos valores copiados
	const float InterpolatedFOV = {FMath::FInterpTo(CurrentFOVCopy, TargetValue, DeltaTime,
									  InterpSpeed) *
								   BezierAlpha};

	// Atualiza o valor de CameraCurrentFOV
	CameraCurrentAimFOV			= InterpolatedFOV;

	return InterpolatedFOV;
}

void AShooterCharacter::CalculateCrosshairSpread(const float &DeltaTime) {

	const Fvc2 WalkSpeedRange		   = {0.f, 600.f};
	const Fvc2 VelocityMultiplierRange = {0.f, 1.f};
	Fvc		   Velocity				   = {GetVelocity()};
	Velocity.Z						   = 0.f;

	// Calculate crosshair velocity factor
	CrosshairVelocityFactor			   = FMath::GetMappedRangeValueClamped(WalkSpeedRange,
				  VelocityMultiplierRange, Velocity.Size());

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

	CrosshairSpreadMultiplier =
	   0.5f + CrosshairVelocityFactor + CrosshairInAirFactor - CrosshairAimFactor +
	   CrosshairShootingFactor;
}

void AShooterCharacter::FireWeapon() {

	if ((EquippedWeapon == nullptr) || (CombatState != ECombatState::ECS_Unoccupied) ||
		(!WeaponHasAmmo()))
		return;

	PlayFireSound();
	SendBullet();
	PlayGunFireMontage();

	// Start Bullet Fire for Crosshairs
	StartCrosshairBulletFire();

	// Decrement Ammo by 1
	EquippedWeapon->DecrementAmmo();

	StartFireTimer();

	if (EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol) {

		// Start Moving slide timer
		EquippedWeapon->StartSlideTimer();
	}
}

bool AShooterCharacter::GetBeanEndLocation(
   const FVector &MuzzleSocketLocation, FVector &OutBeanLocation) {

	// Check for Crosshair Trace Hit
	FHitResult	  CrosshairHitResult = {};
	const bool	  bCrosshairHit		 = {TraceUnderCrosshairs(CrosshairHitResult, OutBeanLocation)};
	FHitResult	  WeaponTraceHit	 = {};
	const FVector WeaponTraceStart	 = {MuzzleSocketLocation};
	const FVector StartToEnd		 = {OutBeanLocation - MuzzleSocketLocation};
	const FVector WeaponTraceEnd	 = {MuzzleSocketLocation + StartToEnd * 1.25f};

	if (bCrosshairHit)
		// Tentative beam location - still need to trace from gun
		OutBeanLocation = CrosshairHitResult.Location;

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd,
	   ECollisionChannel::ECC_Visibility);

	// Object between barrel and BeanEndPoint?
	if (WeaponTraceHit.bBlockingHit) {

		if (BulletHoleDecalMat && BulletHoleDecal) {

			const FVector BulletHoleDecalSize {10.f, 10.f, 10.f};

			BulletHoleDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHoleDecalMat,
			   BulletHoleDecalSize, WeaponTraceHit.ImpactPoint,
			   WeaponTraceHit.ImpactNormal.Rotation(), 5.f);
		}

		constexpr float ImpulseStrength {20'000.f};

		if (WeaponTraceHit.GetComponent() && WeaponTraceHit.GetComponent()->IsSimulatingPhysics()) {

			const FVector Impulse = WeaponTraceHit.ImpactNormal * -1.0f * ImpulseStrength;

			WeaponTraceHit.GetComponent()->AddImpulseAtLocation(
			   Impulse, WeaponTraceHit.ImpactPoint);
		}
		OutBeanLocation = WeaponTraceHit.Location;
		return true;
	}
	return false;
}

void AShooterCharacter::Relative_ControllerRotationYaw(float DeltaTime) {

	// Obtém a velocidade atual do personagem
	const double	CurrentSpeed	  = {GetVelocity().Size()};

	// Define um limite de velocidade mínima para considerar que o personagem
	// está se movendo
	constexpr float MovementThreshold = {10.0f}; // Ajuste conforme necessário

	// Verifica se o personagem está se movendo
	const bool		bIsMoving {CurrentSpeed > MovementThreshold};

	bUseControllerRotationYaw = (bIsMoving && !bIsFiringWeapon) ? true : false;

	if (!((bIsFiringWeapon && !bIsMoving && bDidFire) || bAiming)) return;

	// Obtém a rotação atual do controlador
	const FRotator ControllerRotation = Controller->GetControlRotation();

	// Armazene o yaw atual do controlador
	const float	   CurrentYaw		  = ControllerRotation.Yaw;
	FVector		   MoveDirection	  = ControllerRotation.Vector();

	// Normaliza a direção para ter comprimento 1 (para manter uma
	// velocidade de movimento constante)
	MoveDirection.Normalize();

	// Ajuste a velocidade de movimento conforme necessário
	constexpr float MoveSpeed	   = {1.0f}; // Ajuste conforme necessário

	// Calcule o vetor de movimento multiplicando a direção pela velocidade
	const FVector	MovementVector = MoveDirection * MoveSpeed;

	//&& CurrentYaw != PreviousYaw

	if (!(bAiming || FMath::Abs(CurrentYaw))) return;

	bUseControllerRotationYaw = true;

	// Chame uma função para mover o personagem apenas quando o yaw for
	// maior que 40 graus
	if (GetCharacterMovement()) GetCharacterMovement()->AddForce(MovementVector);

	PreviousYaw = CurrentYaw;
}

void AShooterCharacter::SetupFollowCamera() {

	CheckPtr(FollowCamera);

	CameraDefaultAimFOV = GetFollowCamera()->FieldOfView;
	CameraCurrentAimFOV = CameraDefaultAimFOV;
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
	PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this,
	   &AShooterCharacter::ToggleCinematicCamera);

	/* Fire Weapon */ {
		PlayerInputComponent->BindAction("FireButton", IE_Pressed, this,
		   &AShooterCharacter::FireButtonPressed);

		PlayerInputComponent->BindAction("FireButton", IE_Released, this,
		   &AShooterCharacter::FireButtonReleased);
	}

	/* Aim Weapon */ {
		PlayerInputComponent->BindAction("AimingButton", IE_Pressed, this,
		   &AShooterCharacter::AimingButtonPressed);

		PlayerInputComponent->BindAction("AimingButton", IE_Released, this,
		   &AShooterCharacter::AimingButtonReleased);
	}

	/* Select */ {
		PlayerInputComponent->BindAction("Select", IE_Pressed, this,
		   &AShooterCharacter::SelectButtonPressed);

		PlayerInputComponent->BindAction("Select", IE_Released, this,
		   &AShooterCharacter::SelectButtonReleased);
	}

	// Reload Weapon
	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this,
	   &AShooterCharacter::ReloadButtonPressed);
	// TODO: organize code
	// Crouch
	PlayerInputComponent->BindAction(
	   "Crouch", IE_Pressed, this, &AShooterCharacter::CrouchButtonPressed);

	/* Letter keys */ {

		PlayerInputComponent->BindAction(
		   "Fkey", IE_Pressed, this, &AShooterCharacter::KEY_FkeyPressed);

		PlayerInputComponent->BindAction(
		   "DKey", IE_Pressed, this, &AShooterCharacter::KEY_DKey_D_Pressed);
		PlayerInputComponent->BindAction(
		   "DKey", IE_Released, this, &AShooterCharacter::KEY_DKey_D_Released);

		PlayerInputComponent->BindAction(
		   "SKey", IE_Pressed, this, &AShooterCharacter::KEY_SKey_S_Pressed);
		PlayerInputComponent->BindAction(
		   "SKey", IE_Released, this, &AShooterCharacter::KEY_SKey_S_Released);

		PlayerInputComponent->BindAction(
		   "WKey", IE_Pressed, this, &AShooterCharacter::KEY_WKey_W_Pressed);
		PlayerInputComponent->BindAction(
		   "WKey", IE_Released, this, &AShooterCharacter::KEY_WKey_W_Released);

		PlayerInputComponent->BindAction(
		   "AKey", IE_Pressed, this, &AShooterCharacter::KEY_AKey_A_Pressed);
		PlayerInputComponent->BindAction(
		   "AKey", IE_Released, this, &AShooterCharacter::KEY_AKey_A_Released);
	}

	/* Number keys */ {

		PlayerInputComponent->BindAction(
		   "1Key", IE_Pressed, this, &AShooterCharacter::KEY_1_OneKeyPressed);

		PlayerInputComponent->BindAction(
		   "2Key", IE_Pressed, this, &AShooterCharacter::KEY_2_TwoKeyPressed);

		PlayerInputComponent->BindAction(
		   "3Key", IE_Pressed, this, &AShooterCharacter::KEY_3_ThreeKeyPressed);

		PlayerInputComponent->BindAction(
		   "4Key", IE_Pressed, this, &AShooterCharacter::KEY_4_FourKeyPressed);

		PlayerInputComponent->BindAction(
		   "5Key", IE_Pressed, this, &AShooterCharacter::KEY_5_FiveKeyPressed);

		PlayerInputComponent->BindAction(
		   "6Key", IE_Pressed, this, &AShooterCharacter::KEY_6_SixKeyPressed);

		PlayerInputComponent->BindAction(
		   "7Key", IE_Pressed, this, &AShooterCharacter::KEY_7_SevenKeyPressed);

		PlayerInputComponent->BindAction(
		   "8Key", IE_Pressed, this, &AShooterCharacter::KEY_8_EightKeyPressed);

		PlayerInputComponent->BindAction(
		   "9Key", IE_Pressed, this, &AShooterCharacter::KEY_9_NineKeyPressed);

		PlayerInputComponent->BindAction(
		   "0Key", IE_Pressed, this, &AShooterCharacter::KEY_0_ZeroKeyPressed);
	}

	// Switch to Previous Weapon
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this,
	   &AShooterCharacter::PreviousSlot);
}

void AShooterCharacter::DefaultConstructor_SetupMesh() {

	constexpr const static TCHAR *SkeletalMeshContainerPath = {
	   L"/Script/Engine.SkeletalMesh'/Game/ParagonLtBelica/Characters/Heroes/"
	   L"Belica/Meshes/Belica.Belica'"};

	SkeletalMeshContainer = MakeUnique<ConstructorHelpers::FObjectFinder<USkeletalMesh>>(
	   SkeletalMeshContainerPath);

	PlayerMesh = GetMesh();

	CheckPtr(PlayerMesh);

	if (!SkeletalMeshContainer->Succeeded())
		ExitGameErr(
		   "AShooterCharacter::DefaultConstructor_SetupMesh(): "
		   "SkeletalMeshContainer failed");

	PlayerMesh->SetSkeletalMesh(SkeletalMeshContainer->Object);

	PlayerMesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	PlayerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	PlayerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShooterCharacter::DefaultConstructor_SetCombatCues() {

	if (HipFireMontage == nullptr) {

		HipFireMontage = CDSubObj<UAnimMontage>(L"HipFireMontage");

		constexpr const static TCHAR *HipFireMontagePath =
		   L"/Script/Engine.AnimMontage'/Game/_Game/Character/Animations/"
		   L"HipFireMontage.HipFireMontage'";

		const auto static M_Animation_HipFireMontage {
		   ConstructorHelpers::FObjectFinder<UAnimMontage>(HipFireMontagePath)};

		if (M_Animation_HipFireMontage.Succeeded())
			HipFireMontage = M_Animation_HipFireMontage.Object;
		else {
			ExitGameErr(
			   "AShooterCharacter::DefaultConstructor_SetCombatCues(): "
			   "M_Animation_HipFireMontage failed");
		}
	}

	if (ImpactParticles == nullptr) {

		ImpactParticles = CDSubObj<UParticleSystem>(L"ImpactParticles");

		constexpr const static TCHAR *ImpactParticlesPath =
		   L"/Script/Engine.ParticleSystem'/Game/ParagonLtBelica/FX/Particles/"
		   L"Belica/Abilities/Primary/FX/P_BelicaHitWorld.P_BelicaHitWorld'";

		const auto static M_ImpactParticle {
		   ConstructorHelpers::FObjectFinder<UParticleSystem>(ImpactParticlesPath)};

		if (M_ImpactParticle.Succeeded())
			ImpactParticles = M_ImpactParticle.Object;
		else {
			ExitGameErr(
			   "AShooterCharacter::DefaultConstructor_SetCombatCues(): "
			   "M_ImpactParticle failed");
		}
	}

	if (BeamParticles == nullptr) {

		BeamParticles = CDSubObj<UParticleSystem>(L"BeamParticles");

		constexpr const static TCHAR *BeamParticlesPath =
		   L"/Script/Engine.ParticleSystem'/Game/_Game/Assets/FX/SmokeBean/"
		   L"P_SmokeTrail_Faded.P_SmokeTrail_Faded'";

		const auto static M_BeamParticle {
		   ConstructorHelpers::FObjectFinder<UParticleSystem>(BeamParticlesPath)};

		if (M_BeamParticle.Succeeded())
			BeamParticles = M_BeamParticle.Object;
		else {
			ExitGameErr(
			   "AShooterCharacter::DefaultConstructor_SetCombatCues(): "
			   "M_BeamParticle failed");
		}
	}
}

void AShooterCharacter::DefaultConstructor_SetCharacter() {

	// Don't rotate when the controller rotates. Let that just affect the
	// camera.
	bUseControllerRotationPitch						   = false;
	bUseControllerRotationYaw						   = false;
	bUseControllerRotationRoll						   = false;
	AutoPossessPlayer								   = EAutoReceiveInput::Player0;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement  = false;
	GetCharacterMovement()->RotationRate			   = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity			   = 600.f;
	GetCharacterMovement()->AirControl				   = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed			   = 1500.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched	   = 50.f;
	GetCharacterMovement()->MaxAcceleration			   = 1020.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 50.f;
	GetCharacterMovement()->GroundFriction			   = 1.5f;
}

void AShooterCharacter::DefaultConstructor_InterpComponents() {

	WeaponInterpComp = CDSubObj<USceneComponent>(L"Weapon Interpolation Component");
	CheckPtr(WeaponInterpComp);
	WeaponInterpComp->SetupAttachment(GetFollowCamera());

	InterpComp1 = CDSubObj<USceneComponent>(L"Interpolation Component 1");
	CheckPtr(InterpComp1);
	InterpComp1->SetupAttachment(GetFollowCamera());

	InterpComp2 = CDSubObj<USceneComponent>(L"Interpolation Component 2");
	CheckPtr(InterpComp2);
	InterpComp2->SetupAttachment(GetFollowCamera());

	InterpComp3 = CDSubObj<USceneComponent>(L"Interpolation Component 3");
	CheckPtr(InterpComp3);
	InterpComp3->SetupAttachment(GetFollowCamera());

	InterpComp4 = CDSubObj<USceneComponent>(L"Interpolation Component 4");
	CheckPtr(InterpComp4);
	InterpComp4->SetupAttachment(GetFollowCamera());

	InterpComp5 = CDSubObj<USceneComponent>(L"Interpolation Component 5");
	CheckPtr(InterpComp5);
	InterpComp5->SetupAttachment(GetFollowCamera());

	InterpComp6 = CDSubObj<USceneComponent>(L"Interpolation Component 6");
	CheckPtr(InterpComp6);
	InterpComp6->SetupAttachment(GetFollowCamera());
}

void AShooterCharacter::DefaultConstructor_CameraBoom() {

	// Create a camera boom (pulls in towards the character if there is a
	// collision)
	CameraBoom = CDSubObj<USpringArmComponent>(L"Camera Boom");
	CameraBoom->SetupAttachment(RootComponent);
	// The camera follows at this distance
	CameraBoom->TargetArmLength			= 200.f;
	// behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate at the arm base on the controller
}

void AShooterCharacter::DefaultConstructor_FollowCamera() {

	// Create a Follow Camera
	FollowCamera = CDSubObj<UShooterCharacterCamera>(L"FollowCamera");

	// attach camera to the end of boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AShooterCharacter::Toggle_bIsFiringWeapon_On() {

	bIsFiringWeapon = !bIsFiringWeapon;
	bDidFire		= !bDidFire;
}

void AShooterCharacter::Toggle_bIsFiringWeapon_Off() {

	bIsFiringWeapon = false;
	bDidFire		= false;
}

void AShooterCharacter::AimingButtonPressed() {

	bAiming				 = true;
	bAimingButtonPressed = true;

	if (CombatState != ECombatState::ECS_Reloading && CombatState != ECombatState::ECS_Equipping)
		Aim();

	SetMovingDirection();
}

void AShooterCharacter::AimingButtonReleased() {

	bAiming				 = false;
	bAimingButtonPressed = false;
	StopAiming();
	SetMovingDirection();
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

	CameraBoom->bEnableCameraLag		 = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->bUseCameraLagSubstepping = true;
	CameraBoom->CameraLagSpeed			 = 6.f;
	CameraBoom->CameraRotationLagSpeed	 = 10.f;
	CameraBoom->CameraLagMaxTimeStep	 = 0.016667f;
}

void AShooterCharacter::DisableCameraLagWhenMovingRight(float DeltaTime) {

	const static double ExtraRightSocket {300.0};

	if (bDKey_Pressed) {

		PrintOnScr("bDKey_Pressed")

		   CameraBoom->CameraLagMaxDistance =
		   FMath::FInterpTo(CameraBoom->CameraLagMaxDistance, 220.f, DeltaTime, 3.f);

		const static double NewSocketOffsetY {CameraBoom->SocketOffset.Y + ExtraRightSocket};

		const Fvc			NewSocketOffset = {
			 Fvc(CameraBoom->SocketOffset.X, NewSocketOffsetY, CameraBoom->SocketOffset.Z)};

		CameraBoom->SocketOffset =
		   FMath::VInterpTo(CameraBoom->SocketOffset, NewSocketOffset, DeltaTime, 30.f);
	}

	else if (bDKey_Released) {

		CameraBoom->CameraLagMaxDistance =
		   FMath::FInterpTo(CameraBoom->CameraLagMaxDistance, 200.f, DeltaTime, 50.f);

		const Fvc NewSocketOffset {(CameraBoom->SocketOffset.X,
		   CameraBoom->SocketOffset.Y - ExtraRightSocket, CameraBoom->SocketOffset.Z)};

		CameraBoom->SocketOffset =
		   FMath::VInterpTo(CameraBoom->SocketOffset, NewSocketOffset, DeltaTime, 50.f);
	}

	else if (bSKey_Pressed) {

		CameraBoom->CameraLagMaxDistance =
		   FMath::FInterpTo(CameraBoom->CameraLagMaxDistance, 10.f, DeltaTime, 3.f);

		CameraBoom->SocketOffset =
		   FMath::VInterpTo(CameraBoom->SocketOffset, FVector(-80.f, 87.f, 27.f), DeltaTime, 50.f);
	}

	else if (bSKey_Released) {

		CameraBoom->CameraLagMaxDistance =
		   FMath::FInterpTo(CameraBoom->CameraLagMaxDistance, 200.f, DeltaTime, 100.f);

		CameraBoom->SocketOffset =
		   FMath::VInterpTo(CameraBoom->SocketOffset, FVector(0.f, 87.f, 27.f), DeltaTime, 5.f);
	}
}

void AShooterCharacter::DisableCameraLagWhenMovingBackwards(float DeltaTime) {

	if (bSKey_Pressed) {

		CameraBoom->CameraLagMaxDistance = FMath::FInterpTo(CameraBoom->CameraLagMaxDistance,
		   10.f, DeltaTime, 3.f);
		CameraBoom->SocketOffset		 = FMath::VInterpTo(CameraBoom->SocketOffset,
				   FVector(-80.f, 87.f, 27.f), DeltaTime, 0.5f);

	} else {

		CameraBoom->CameraLagMaxDistance = FMath::FInterpTo(CameraBoom->CameraLagMaxDistance,
		   200.f, DeltaTime, 100.f);
		CameraBoom->SocketOffset		 = FMath::VInterpTo(CameraBoom->SocketOffset,
				   FVector(0.f, 87.f, 27.f), DeltaTime, 50.f);
	}
}

void AShooterCharacter::CinematicCameraOff() {

	CameraBoom->bEnableCameraLag		 = false;
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->bUseCameraLagSubstepping = false;
}

void AShooterCharacter::MoveForward(float Value) {

	if (!(Controller && Value != 0.0f)) return;

	// find which way is forward
	const FRotator Rotation	   = {Controller->GetControlRotation()};
	const FRotator YawRotation = {0, Rotation.Yaw, 0};
	const FVector  X		   = {FRotationMatrix {YawRotation}.GetUnitAxis(EAxis::X)};
	const FVector  Direction   = {X};

	AddMovementInput(Direction, Value);
}

void AShooterCharacter::MoveRight(float Value) {

	if (!(Controller && Value != 0.0f)) return;

	// find which way is right
	const FRotator Rotation	   = {Controller->GetControlRotation()};
	const FRotator YawRotation = {0, Rotation.Yaw, 0};
	const FVector  Y		   = {FRotationMatrix {YawRotation}.GetUnitAxis(EAxis::Y)};
	const FVector  Direction   = {Y};

	AddMovementInput(Direction, Value);
}

void AShooterCharacter::DefaultConstructor_CustomCamera() {

	if (!bUseCustomCamera) return;

	// Create a Follow Camera
	CustomCamera = CDSubObj<UShooterCharacterCamera>(L"CustomCamera");

	// attach camera to the end of boom
	CustomCamera->SetupAttachment(CustomCamera->GetCustomCameraBoom(),
	   USpringArmComponent::SocketName);

	// Camera does not rotate relative to arm
	CustomCamera->bUsePawnControlRotation = false;
}

void AShooterCharacter::TurnAtRate(float Rate) {

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	/* deg\sec multiply by sec\frame */
}

void AShooterCharacter::LookUpAtRate(float Rate) {
	AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Turn(float Value) {

	const float TurnScaleFactor {(bAiming) ? MouseAimingTurnRate : MouseHipTurnRate};
	AddControllerYawInput(Value * TurnScaleFactor);
}

void AShooterCharacter::LookUp(float Value) {

	const float LookUpScaleFactor {(bAiming) ? MouseAimingLookUpRate : MouseHipLookUpRate};
	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AShooterCharacter::StartCrosshairBulletFire() {

	bFiringBullet = true;
	GetWorldTimerManager().SetTimer(CrosshairShootTimer, this,
	   &AShooterCharacter::FinishCrosshairBulletFire, ShootTimeDuration);
}

void AShooterCharacter::FinishCrosshairBulletFire() { bFiringBullet = false; }

void AShooterCharacter::FireButtonPressed() {

	bFireButtonPressed = true;
	FireWeapon();
	SetMovingDirection();
}

void AShooterCharacter::FireButtonReleased() {
	bFireButtonPressed = false;
	SetMovingDirection();
}

void AShooterCharacter::StartFireTimer() {

	if (EquippedWeapon == nullptr) return;

	CombatState = ECombatState::ECS_FireTimerInProgress;

	if (bShouldFire) {

		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AShooterCharacter::AutoFireReset,
		   EquippedWeapon->GetAutoFireRate());
	}
}

void AShooterCharacter::AutoFireReset() {

	CombatState = ECombatState::ECS_Unoccupied;
	CheckPtr(EquippedWeapon);

	if (WeaponHasAmmo()) {

		if (bFireButtonPressed && EquippedWeapon->GetAutomatic()) FireWeapon();

	} else {

		ReloadWeapon();
	}
}

bool AShooterCharacter::TraceUnderCrosshairs(FHitResult &OutHitResult, FVector &OutHitlocation) {

	// Get Viewport Size
	FVector2D ViewPortSize			  = {};
	// Get Screen Space of Crosshair Location
	FVector2D CrosshairLocation		  = {ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f};
	FVector	  CrosshairWorldPosition  = {};
	FVector	  CrosshairWorldDirection = {};

	if (GEngine && GEngine->GameViewport) GEngine->GameViewport->GetViewportSize(ViewPortSize);

	CrosshairLocation.Y -= CrosshairHeight;

	// Get World position and direction of Crosshair
	const bool bScreenToWorld {UGameplayStatics::DeprojectScreenToWorld(
	   UGameplayStatics::GetPlayerController(this, 0), CrosshairLocation, CrosshairWorldPosition,
	   CrosshairWorldDirection)};

	if (bScreenToWorld) {

		// Trace from Crosshair world location outward
		const FVector Start = {CrosshairWorldPosition};
		const FVector End	= {Start + CrosshairWorldDirection * 50'000.f};
		OutHitlocation		= End;

		// Trace outward from Crosshair world location
		if (GetWorld())
			GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End,
			   ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit) {

			OutHitlocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void AShooterCharacter::TraceForItems() {

	if (bShouldTraceForItems) {

		FHitResult ItemHitResult = {};
		FVector	   HitLocation	 = {};
		TraceUnderCrosshairs(ItemHitResult, HitLocation);

		if (ItemHitResult.bBlockingHit) {

			// Cast to AItem to check if it's valid and if it has a widget
			TraceHitItem				  = Cast<AItem>(ItemHitResult.GetActor());

			const AWeapon *TraceHitWeapon = Cast<AWeapon>(TraceHitItem);

			if (TraceHitWeapon) {

				if (HighlightedSlot == -1)
					// If not currently highlighting a slot, highlight the slot
					HighlightInventorySlot();

			} else {

				// is a slot being highlighted?
				if (HighlightedSlot != -1)
					// Unhighlight the slot
					UnHighlightInventorySlot();
			}

			// if Hit Item is in EquipInterping state set to nullptr
			if (TraceHitItem && TraceHitItem->GetItemState() == EItemState::EIS_EquipInterping)
				TraceHitItem = nullptr;

			if (TraceHitItem && TraceHitItem->GetPickupWidget()) {

				// Show the pickup widget
				TraceHitItem->GetPickupWidget()->SetVisibility(true);

				TraceHitItem->EnableCustomDepth();

				if (Inventory.Num() >= InventoryCapacity)
					// Inventory is full
					TraceHitItem->SetCharacterInventoryFull(true);

				else
					// Inventory is not full
					TraceHitItem->SetCharacterInventoryFull(false);
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

void AShooterCharacter::EquipWeapon(AWeapon *WeaponToEquip, bool bSwapping) {

	CheckPtr(WeaponToEquip)

	   // Get the Hand Socket
	   const USkeletalMeshSocket *HandSocket = GetMesh()->GetSocketByName(
		  FName(L"RightHandSocket"));

	CheckPtr(HandSocket);

	if (HandSocket)
		// Attach the Weapon to the hand socket RightHandSocket
		HandSocket->AttachActor(WeaponToEquip, GetMesh());
	else {
		ExitGame();
		return;
	}

	if (EquippedWeapon == nullptr) {

		// -1 == no EquippedWeapon yet. No need to reverse the icon animation
		EquipItemDelegate.Broadcast(-1, WeaponToEquip->GetSlotIndex());

	} else if (!bSwapping) {

		EquipItemDelegate.Broadcast(EquippedWeapon->GetSlotIndex(), WeaponToEquip->GetSlotIndex());
	}

	// Set EquippedWeapon to the newly spawned Weapon
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
}

void AShooterCharacter::DropWeapon() {

	CheckPtr(EquippedWeapon);

	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

	EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
	EquippedWeapon->SetItemState(EItemState::EIS_Falling);
	EquippedWeapon->ThrowWeapon();
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
	EquipWeapon(WeaponToSwap, true);

	TraceHitItem		  = nullptr;
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

	CheckPtr(EquippedWeapon->GetFireSound());
	UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetFireSound());
}

void AShooterCharacter::SendBullet() {

	const USkeletalMeshSocket *BarrelSocket {EquippedWeapon->GetItemMesh()->GetSocketByName(
	   "BarrelSocket")};

	CheckPtr(BarrelSocket);

	const FTransform SocketTransform {BarrelSocket->GetSocketTransform(
	   EquippedWeapon->GetItemMesh())};

	CheckPtr(EquippedWeapon->GetMuzzleFlash());

	UGameplayStatics::SpawnEmitterAtLocation(
	   GetWorld(), EquippedWeapon->GetMuzzleFlash(), SocketTransform);

	FVector	   BeamEnd {};

	// Get Beam End Location
	const bool bBeanEnd {GetBeanEndLocation(SocketTransform.GetLocation(), BeamEnd)};

	if (bBeanEnd) return;

	if (ImpactParticles)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamEnd);

	UParticleSystemComponent *Beam {UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
	   BeamParticles, SocketTransform)};

	if (Beam) Beam->SetVectorParameter(FName("Target"), BeamEnd);
}

void AShooterCharacter::PlayGunFireMontage() {

	UAnimInstance *AnimInstance {GetMesh()->GetAnimInstance()};

	CheckPtr(AnimInstance);
	CheckPtr(HipFireMontage);

	AnimInstance->Montage_Play(HipFireMontage);
	AnimInstance->Montage_JumpToSection(FName("StartFire"));
}

void AShooterCharacter::ReloadButtonPressed() { ReloadWeapon(); }

void AShooterCharacter::ReloadWeapon() {

	if ((CombatState != ECombatState::ECS_Unoccupied) || (EquippedWeapon == nullptr) ||
		(!CarryingAmmo()) || (EquippedWeapon->ClipIsFull()))
		return;

	// Do we have ammo of the correct type?
	if (!((CarryingAmmo()) && (!EquippedWeapon->ClipIsFull()))) return;

	if (bAiming) StopAiming();

	CombatState					= ECombatState::ECS_Reloading;
	UAnimInstance *AnimInstance = {GetMesh()->GetAnimInstance()};

	CheckPtr(ReloadMontage);
	CheckPtr(AnimInstance);

	AnimInstance->Montage_Play(ReloadMontage);
	AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
}

void AShooterCharacter::FinishReloading() {

	// Update the combat state
	CombatState = ECombatState::ECS_Unoccupied;

	if (bAimingButtonPressed) Aim();

	if (EquippedWeapon == nullptr) return;

	const auto AmmoType {EquippedWeapon->GetAmmoType()};

	// Update the ammo map
	if (!(AmmoMap.Contains(AmmoType))) return;

	// Amount of ammo the character is carrying of the equipped weapons type
	int32		CarriedAmmo	  = {AmmoMap [AmmoType]};

	// Space left in the magazine of the equipped weapon
	const int32 MagEmptySpace = {EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo()};

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

void AShooterCharacter::FinishEquipping() {

	// Update the combat state
	CombatState = ECombatState::ECS_Unoccupied;
	if (bAimingButtonPressed) Aim();
}

bool AShooterCharacter::CarryingAmmo() {

	if (EquippedWeapon == nullptr) return false;

	const EAmmoType AmmoType {EquippedWeapon->GetAmmoType()};

	if (AmmoMap.Contains(AmmoType)) return AmmoMap [AmmoType] > 0;

	return false;
}

void AShooterCharacter::GrabClip() {

	if ((EquippedWeapon == nullptr) || (HandSceneComponent == nullptr)) return;

	// Index for the clip bone on the EquippedWeapon
	const int32 ClipBoneIndex {EquippedWeapon->GetItemMesh()->GetBoneIndex(
	   EquippedWeapon->GetClipBoneName())};

	// Store the transform of the clip bone
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	const FAttachmentTransformRules AttachmentRules {EAttachmentRule::KeepRelative, true};

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

	SetMovingDirection();
}

void AShooterCharacter::Jump() {

	if (bCrouching) {

		bCrouching							 = false;
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	} else {

		bIsJumping = true;
		ACharacter::Jump();
	}
	SetMovingDirection();
}

void AShooterCharacter::StopJumping() {

	ACharacter::StopJumping();
	bIsJumping = false;
	SetMovingDirection();
}

void AShooterCharacter::SelectButtonPressed() {

	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (TraceHitItem) {

		TraceHitItem->StartItemCurve(this, true);
		TraceHitItem = nullptr;
	}
}

void AShooterCharacter::SelectButtonReleased() {}

void AShooterCharacter::GetOriginalCameraLagOffset() {

	if (OriginalCameraSocketOffset == FVector::ZeroVector)
		OriginalCameraSocketOffset = CameraBoom->SocketOffset;

	OriginalCameraLagSpeed		 = CameraBoom->CameraLagSpeed;
	OriginalCameraLagMaxDistance = CameraBoom->CameraLagMaxDistance;
}

void AShooterCharacter::AdjustVectors() {

	if (OffsetFroward.X == 0) OffsetFroward.X = CameraBoom->SocketOffset.X;
	if (OffsetFroward.Y == 0) OffsetFroward.Y = CameraBoom->SocketOffset.Y;
	if (OffsetFroward.Z == 0) OffsetFroward.Z = CameraBoom->SocketOffset.Z;

	if (OffsetBackwards.X == 0) OffsetBackwards.X = CameraBoom->SocketOffset.X;
	if (OffsetBackwards.Y == 0) OffsetBackwards.Y = CameraBoom->SocketOffset.Y;
	if (OffsetBackwards.Z == 0) OffsetBackwards.Z = CameraBoom->SocketOffset.Z;

	if (OffsetRight.X == 0) OffsetRight.X = CameraBoom->SocketOffset.X;
	if (OffsetRight.Y == 0) OffsetRight.Y = CameraBoom->SocketOffset.Y;
	if (OffsetRight.Z == 0) OffsetRight.Z = CameraBoom->SocketOffset.Z;
}

float AShooterCharacter::GetCrosshairHeight() const { return CrosshairHeight; }

float AShooterCharacter::GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

void  AShooterCharacter::IncrementOverlappedItemCount(int8 Amount) {

	 if (OverlappedItemCount + Amount <= 0) {

		 OverlappedItemCount  = 0;
		 bShouldTraceForItems = false;

	 } else {

		 OverlappedItemCount += Amount;
		 bShouldTraceForItems = true;
	 }
}

FVector AShooterCharacter::GetCameraInterpLocation() {

	const FVector CameraWorldLocation = {FollowCamera->GetComponentLocation()};
	const FVector CameraForward		  = {FollowCamera->GetForwardVector()};
	const FVector CameraInterp		  = {FVector(0.f, 0.f, CameraInterpElevation)};

	return CameraWorldLocation + CameraForward * CameraInterpDistance + CameraInterp;
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
	const auto Ammo = Cast<AAmmo>(Item);
	if (Ammo) PickupAmmo(Ammo);
}

void AShooterCharacter::InitializeInterpLocations() {

	const FInterpLocation WeaponLocation {WeaponInterpComp, 0};
	InterpLocations.Add(WeaponLocation);

	const FInterpLocation InterpLoc1 {InterpComp1, 0};
	InterpLocations.Add(InterpLoc1);

	const FInterpLocation InterpLoc2 {InterpComp2, 0};
	InterpLocations.Add(InterpLoc2);

	const FInterpLocation InterpLoc3 {InterpComp3, 0};
	InterpLocations.Add(InterpLoc3);

	const FInterpLocation InterpLoc4 {InterpComp4, 0};
	InterpLocations.Add(InterpLoc4);

	const FInterpLocation InterpLoc5 {InterpComp5, 0};
	InterpLocations.Add(InterpLoc5);

	const FInterpLocation InterpLoc6 {InterpComp6, 0};
	InterpLocations.Add(InterpLoc6);
}

inline void AShooterCharacter::ResetPickupSoundTimer() { bShouldPlayPickupSound = true; }

inline void AShooterCharacter::ResetEquipSoundTimer() { bShouldPlayEquipSound = true; }

void		AShooterCharacter::ExchangeInventoryItems(int32 CurrentItemindex, int32 NewItemIndex) {

	   const bool CanExchange {(
		  (CurrentItemindex != NewItemIndex) && (NewItemIndex < Inventory.Num()) &&
		  ((CombatState == ECombatState::ECS_Unoccupied) ||
			 (CombatState == ECombatState::ECS_Equipping)) &&
		  (bExchangeInventoryItensEnabled))};

	   if (!CanExchange) return;

	   if (bAiming) StopAiming();

	   LastSlotIndex				  = CurrentItemindex;
	   LastLastSlotIndex			  = NewItemIndex;
	   bExchangeInventoryItensEnabled = false;
	   AWeapon *OldEquippedWeapon	  = {EquippedWeapon};
	   AWeapon *NewWeapon			  = {Cast<AWeapon>(Inventory [NewItemIndex])};

	   EquipWeapon(NewWeapon);

	   OldEquippedWeapon->SetItemState(EItemState::EIS_PickedUp);
	   NewWeapon->SetItemState(EItemState::EIS_Equipped);
	   CombatState = ECombatState::ECS_Equipping;
	   UAnimInstance *AnimInstance {nullptr};
	   AnimInstance = GetMesh()->GetAnimInstance();
	   CheckPtr(AnimInstance);
	   CheckPtr(EquipMontage);
	   AnimInstance->Montage_Play(EquipMontage, 1.0f);
	   AnimInstance->Montage_JumpToSection(FName(L"Equip"));
	   GetWorld()->GetTimerManager().SetTimer(ExchangeInventoryItensTimer, this,
			  &AShooterCharacter::EnableExchangeInventoryItems, ExchangeInventoryItensTime, false);
	   NewWeapon->PlayEquipSound(true);
}

void AShooterCharacter::PreviousSlot() { ExchangeInventoryItems(LastLastSlotIndex, LastSlotIndex); }

void AShooterCharacter::EnableExchangeInventoryItems() { bExchangeInventoryItensEnabled = true; }

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

int32 AShooterCharacter::GetEmptyInventorySlot() {

	for (int32 i = 0; i < Inventory.Num(); i++) {

		if (Inventory [i] == nullptr) return i;
	}

	if (Inventory.Num() < InventoryCapacity) return Inventory.Num();
	return -1; // inventory is full
}

void AShooterCharacter::HighlightInventorySlot() {

	const int32 EmptySlot {GetEmptyInventorySlot()};

	HighlightIconDelegate.Broadcast(EmptySlot, true);
	HighlightedSlot = EmptySlot;
}

void AShooterCharacter::UnHighlightInventorySlot() {

	HighlightIconDelegate.Broadcast(HighlightedSlot, false);
	HighlightedSlot = -1;
}

FInterpLocation AShooterCharacter::GetInterpLocation(int32 Index) {

	if (Index <= InterpLocations.Num()) { return InterpLocations [Index]; }
	return FInterpLocation();
}

void AShooterCharacter::IncrementInterpLocItemCount(int32 Index, int32 Amount) {

	if (Amount < -1 || Amount > 1) return;

	if (InterpLocations.Num() >= Index) { InterpLocations [Index].ItemCount += Amount; }
}

void AShooterCharacter::HandleMouseWheel(float Value) {

	if (Value != 0.0f) {
		if (Value > 0.0f)
			ScrollUp();
		else
			ScrollDown();
	}
}

void AShooterCharacter::DebugSlotsItems() {

	UpdateSlotsItems();

	if (GEngine) {

		GEngine->AddOnScreenDebugMessage(
		   1, -1.f, FColor::Red, FString::Printf(L"Current Slot Index: %d", CurrentSlotIndex));
		GEngine->AddOnScreenDebugMessage(
		   2, -1.f, FColor::Blue, FString::Printf(L"Inventory Count: %d", InventoryCount));
	}
}

void AShooterCharacter::UpdateSlotsItems() {

	CurrentSlotIndex = EquippedWeapon->GetSlotIndex();
	InventoryCount	 = Inventory.Num();
}

void AShooterCharacter::EquipWeapon() {

	// Spawn Default Weapon and equip it
	EquipWeapon(SpawnDefaultWeapon());
	Inventory.Add(EquippedWeapon);
	EquippedWeapon->SetSlotIndex(0);
	EquippedWeapon->DisableCustomDepth();
	EquippedWeapon->DisableGlowMaterial();
	EquippedWeapon->SetCharacter(this);
}

void AShooterCharacter::ScrollUp() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();

	if (InventoryCount == 1) return;

	// Go to the first slot index
	else if ((CurrentSlotIndex >= EquippedWeapon->GetMaxSlotNumber()) ||
			 ((CurrentSlotIndex + 1) == InventoryCount))
		ExchangeInventoryItems(CurrentSlotIndex, 0);

	// advance the slot index
	else if ((InventoryCount - 1) > CurrentSlotIndex)
		ExchangeInventoryItems(CurrentSlotIndex, (CurrentSlotIndex + 1));
}

void AShooterCharacter::ScrollDown() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();

	if (InventoryCount == 1) return;

	// Go to the last slot index
	else if (CurrentSlotIndex <= 0)
		return ExchangeInventoryItems(CurrentSlotIndex, (InventoryCount - 1));

	// Go back the slot index
	else if ((InventoryCount - 1) >= CurrentSlotIndex)
		return ExchangeInventoryItems(CurrentSlotIndex, (CurrentSlotIndex - 1));
}

void AShooterCharacter::KEY_DKey_D_Pressed() { KeyMethodDKey(); }
void AShooterCharacter::KeyMethodDKey() {

	bDKey_Released = false;
	bDKey_Pressed  = true;
	SetMovingDirection();
}

void AShooterCharacter::KEY_DKey_D_Released() { KeyMethodDKeyReleased(); }
void AShooterCharacter::KeyMethodDKeyReleased() {

	bDKey_Released = true;
	bDKey_Pressed  = false;
	SetMovingDirection();
}

void AShooterCharacter::KEY_SKey_S_Pressed() { KeyMethodSKey(); }
void AShooterCharacter::KeyMethodSKey() {

	bSKey_Pressed  = true;
	bSKey_Released = false;
	SetMovingDirection();
}

void AShooterCharacter::KEY_SKey_S_Released() { KeyMethodSKeyReleased(); }
void AShooterCharacter::KeyMethodSKeyReleased() {

	bSKey_Pressed  = false;
	bSKey_Released = true;
	SetMovingDirection();
}

void AShooterCharacter::KEY_WKey_W_Pressed() { KeyMethodWKey(); }
void AShooterCharacter::KeyMethodWKey() {

	bWKey_Pressed  = true;
	bWKey_Released = false;
	SetMovingDirection();
}

void AShooterCharacter::KEY_WKey_W_Released() { KeyMethodWKeyReleased(); }
void AShooterCharacter::KeyMethodWKeyReleased() {

	bWKey_Pressed  = false;
	bWKey_Released = true;
	SetMovingDirection();
}

void AShooterCharacter::KEY_AKey_A_Pressed() { KeyMethodAKey(); }
void AShooterCharacter::KeyMethodAKey() {

	bAKey_Pressed  = true;
	bAKey_Released = false;
	SetMovingDirection();
}

void AShooterCharacter::KEY_AKey_A_Released() { KeyMethodAKeyReleased(); }
void AShooterCharacter::KeyMethodAKeyReleased() {

	bAKey_Pressed  = false;
	bAKey_Released = true;
	SetMovingDirection();
}

void AShooterCharacter::KEY_FkeyPressed() { KeyMethodFKey(); }
void AShooterCharacter::KeyMethodFKey() {

	CheckPtr(EquippedWeapon)

	   UpdateSlotsItems();
	if (CurrentSlotIndex == 0) return;
	return ExchangeInventoryItems(CurrentSlotIndex, 0);
}

void AShooterCharacter::KEY_1_OneKeyPressed() { KeyMethod1Key(); }
void AShooterCharacter::KeyMethod1Key() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();
	if (CurrentSlotIndex == 1) return;
	return ExchangeInventoryItems(CurrentSlotIndex, 1);
}

void AShooterCharacter::KEY_2_TwoKeyPressed() { KeyMethod2Key(); }
void AShooterCharacter::KeyMethod2Key() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();
	if (CurrentSlotIndex == 2) return;
	return ExchangeInventoryItems(CurrentSlotIndex, 2);
}

void AShooterCharacter::KEY_3_ThreeKeyPressed() { KeyMethod3Key(); }
void AShooterCharacter::KeyMethod3Key() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();
	if (CurrentSlotIndex == 3) return;
	return ExchangeInventoryItems(CurrentSlotIndex, 3);
}

void AShooterCharacter::KEY_4_FourKeyPressed() { KeyMethod4Key(); }
void AShooterCharacter::KeyMethod4Key() {

	CheckPtr(EquippedWeapon);

	UpdateSlotsItems();
	if (CurrentSlotIndex == 4) return;
	return ExchangeInventoryItems(CurrentSlotIndex, 4);
}

void AShooterCharacter::KEY_5_FiveKeyPressed() { KeyMethod5Key(); }
void AShooterCharacter::KeyMethod5Key() {

	if (EquippedWeapon) {

		UpdateSlotsItems();

		if (CurrentSlotIndex == 5) return;
		return ExchangeInventoryItems(CurrentSlotIndex, 5);

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

void AShooterCharacter::SetMovingLogic() {}

void AShooterCharacter::SetMovingDirection() {

	SetMovingLogic();

	bNotMoving = false;

	bool	   bMovingStraight {};
	bool	   bMovingDiagonal {};
	bool	   bMovingStraightAim {};
	bool	   bMovingDiagonalAim {};
	bool	   bMovingStraightCrouch {};
	bool	   bMovingDiagonalCrouch {};
	bool	   bMovingStraightCrouchAim {};
	bool	   bMovingDiagonalCrouchAim {};
	bool	   bAimingStill {};

	const bool bMovingForward		  = {bWKey_Pressed && !bWKey_Released && !bAiming && !bCrouching};
	const bool bMovingBackward		  = {bSKey_Pressed && !bSKey_Released && !bAiming && !bCrouching};
	const bool bMovingRight			  = {bDKey_Pressed && !bDKey_Released && !bAiming && !bCrouching};
	const bool bMovingLeft			  = {bAKey_Pressed && !bAKey_Released && !bAiming && !bCrouching};
	const bool bMovingForwardRight	  = {bMovingForward && bMovingRight && !bAiming && !bCrouching};
	const bool bMovingForwardLeft	  = {bMovingForward && bMovingLeft && !bAiming && !bCrouching};
	const bool bMovingBackwardRight	  = {bMovingBackward && bMovingRight && !bAiming && !bCrouching};
	const bool bMovingBackwardLeft	  = {bMovingBackward && bMovingLeft && !bAiming && !bCrouching};
	const bool bForwardAim			  = {bWKey_Pressed && !bWKey_Released && bAiming && !bCrouching};
	const bool bBackwardAim			  = {bSKey_Pressed && !bSKey_Released && bAiming && !bCrouching};
	const bool bRightAim			  = {bDKey_Pressed && !bDKey_Released && bAiming && !bCrouching};
	const bool bLeftAim				  = {bAKey_Pressed && !bAKey_Released && bAiming && !bCrouching};
	const bool bForwardRightAim		  = {bForwardAim && bRightAim && bAiming && !bCrouching};
	const bool bForwardLeftAim		  = {bForwardAim && bLeftAim && bAiming && !bCrouching};
	const bool bBackwardRightAim	  = {bBackwardAim && bRightAim && bAiming && !bCrouching};
	const bool bBackwardLeftAim		  = {bBackwardAim && bLeftAim && bAiming && !bCrouching};
	const bool bForwardCrouch		  = {bWKey_Pressed && !bWKey_Released && !bAiming && bCrouching};
	const bool bBackwardCrouch		  = {bSKey_Pressed && !bSKey_Released && !bAiming && bCrouching};
	const bool bRightCrouch			  = {bDKey_Pressed && !bDKey_Released && !bAiming && bCrouching};
	const bool bLeftCrouch			  = {bAKey_Pressed && !bAKey_Released && !bAiming && bCrouching};
	const bool bForwardRightCrouch	  = {bForwardCrouch && bRightCrouch && !bAiming && bCrouching};
	const bool bForwardLeftCrouch	  = {bForwardCrouch && bLeftCrouch && !bAiming && bCrouching};
	const bool bBackwardRightCrouch	  = {bBackwardCrouch && bRightCrouch && !bAiming && bCrouching};
	const bool bBackwardLeftCrouch	  = {bBackwardCrouch && bLeftCrouch && !bAiming && bCrouching};
	const bool bForwardCrouchAim	  = {bWKey_Pressed && !bWKey_Released && bAiming && bCrouching};
	const bool bBackwardCrouchAim	  = {bSKey_Pressed && !bSKey_Released && bAiming && bCrouching};
	const bool bRightCrouchAim		  = {bDKey_Pressed && !bDKey_Released && bAiming && bCrouching};
	const bool bLeftCrouchAim		  = {bAKey_Pressed && !bAKey_Released && bAiming && bCrouching};
	const bool bForwardRightCrouchAim = {
	   bForwardCrouchAim && bRightCrouchAim && bAiming && bCrouching};
	const bool bForwardLeftCrouchAim = {
	   bForwardCrouchAim && bLeftCrouchAim && bAiming && bCrouching};
	const bool bBackwardRightCrouchAim = {
	   bBackwardCrouchAim && bRightCrouchAim && bAiming && bCrouching};
	const bool bBackwardLeftCrouchAim = {
	   bBackwardCrouchAim && bLeftCrouchAim && bAiming && bCrouching};

	const bool bStraightDirections {
	   (bMovingForward || bMovingBackward || bMovingRight || bMovingLeft) && !bAiming};

	const bool bDiagonalDirections {
	   (bMovingForwardRight || bMovingForwardLeft || bMovingBackwardRight || bMovingBackwardLeft) &&
	   !bAiming};

	const bool bStraightDirectionsAim {bForwardAim || bBackwardAim || bRightAim || bLeftAim};

	const bool bDiagonalDirectionsAim {
	   bForwardRightAim || bForwardLeftAim || bBackwardRightAim || bBackwardLeftAim};

	const bool bStraightDirectionsCrouch {
	   bForwardCrouch || bBackwardCrouch || bRightCrouch || bLeftCrouch};

	const bool bDiagonalDirectionsCrouch {
	   bForwardRightCrouch || bForwardLeftCrouch || bBackwardRightCrouch || bBackwardLeftCrouch};

	const bool bStraightDirectionsCrouchAim {
	   bForwardCrouchAim || bBackwardCrouchAim || bRightCrouchAim || bLeftCrouchAim};

	const bool bDiagonalDirectionsCrouchAim {
	   bForwardRightCrouchAim || bForwardLeftCrouchAim || bBackwardRightCrouchAim ||
	   bBackwardLeftCrouchAim};

	bMovingStraight		  = (bStraightDirections && !bDiagonalDirections && !bAiming);
	bMovingDiagonal		  = (!bMovingStraight && bDiagonalDirections && !bAiming);
	bMovingStraightAim	  = (bAiming && bStraightDirectionsAim && !bDiagonalDirectionsAim);
	bMovingDiagonalAim	  = (bAiming && !bMovingStraightAim && bDiagonalDirectionsAim);
	bMovingStraightCrouch = (bCrouching && bStraightDirectionsCrouch && !bDiagonalDirectionsCrouch);
	bMovingDiagonalCrouch = (bCrouching && !bMovingStraightCrouch && bDiagonalDirectionsCrouch);

	bMovingStraightCrouchAim =
	   (bCrouching && bAiming && bStraightDirectionsCrouchAim && !bDiagonalDirectionsCrouchAim);

	bMovingDiagonalCrouchAim = (bCrouching && bAiming && !bMovingStraightCrouchAim);
	bNotMoving				 = !(bMovingStraight && bMovingDiagonal && bMovingStraightAim && bMovingDiagonalAim);
	bAimingStill			 = (bNotMoving && bAiming);

	const bool bOnlyMovingStraight {
	   !bAiming && bMovingStraight && !bMovingDiagonal && !bCrouching && !bIsJumping};
	const bool bOnlyMovingDioganaly {
	   !bAiming && bMovingDiagonal && !bMovingStraight && !bCrouching && !bIsJumping};

	const bool bOnlyMovingStraightJump {
	   !bAiming && bMovingStraight && !bMovingDiagonal && !bCrouching && bIsJumping};
	const bool bOnlyMovingDioganalyJump {
	   !bAiming && bMovingDiagonal && !bMovingStraight && !bCrouching && bIsJumping};

	const bool bOnlyMovingStraightAim {
	   bMovingStraightAim && !bMovingDiagonalAim && !bCrouching && !bIsJumping};

	const bool bOnlyMovingDiagonalAim {
	   !bMovingStraightAim && bMovingDiagonalAim && !bCrouching && !bIsJumping};

	const bool bOnlyMovingStraighCrouch {
	   bMovingStraightCrouch && !bMovingDiagonalCrouch && bCrouching && !bIsJumping};
	const bool bOnlyMovingDiagonalyCrouch {
	   !bMovingStraightCrouch && bMovingDiagonalCrouch && bCrouching && !bIsJumping};

	const bool bOnlyMovingStraighCrouchnAim {
	   bMovingStraightCrouchAim && !bMovingDiagonalCrouchAim && bCrouching && bAiming &&
	   !bIsJumping};
	const bool bOnlyMovingDiagonalyCrouchnAim {
	   !bMovingStraightCrouchAim && bMovingDiagonalCrouchAim && bCrouching && bAiming &&
	   !bIsJumping};

	const bool bCrouch		  = {!bAiming && bCrouching && !bIsJumping};
	const bool bAim			  = {bAiming && !bCrouching && !bIsJumping};
	const bool bCrouchAim	  = {bAiming && bCrouching && !bIsJumping};
	const bool bJumping		  = {!bAiming && !bCrouching && bIsJumping};
	const bool bNotMove		  = {!bAiming && !bCrouching && !bIsJumping};
	const bool bSimpleActions = {bCrouch || bAim || bCrouchAim || bJumping || bNotMove};

	// clang-format off
	if (bSimpleActions) {

		if (bCrouchAim) MovingDirection = EMovingDirection::EMD_CrouchAim;
		if (bCrouch)    MovingDirection = EMovingDirection::EMD_Crouch;
		if (bAim)       MovingDirection = EMovingDirection::EMD_Aim;
		if (bJumping)   MovingDirection = EMovingDirection::EMD_Jump;
		if (bNotMove)   MovingDirection = EMovingDirection::EMD_None;
	}

	if (bOnlyMovingStraight) {

		if (bMovingForward)  MovingDirection = EMovingDirection::EMD_Forward;
		if (bMovingBackward) MovingDirection = EMovingDirection::EMD_Backward;
		if (bMovingRight)    MovingDirection = EMovingDirection::EMD_Right;
		if (bMovingLeft)     MovingDirection = EMovingDirection::EMD_Left;
	}

	if (bOnlyMovingDioganaly) {

		if (bMovingForwardRight)  MovingDirection = EMovingDirection::EMD_ForwardRight;
		if (bMovingForwardLeft)   MovingDirection = EMovingDirection::EMD_ForwardLeft;
		if (bMovingBackwardRight) MovingDirection = EMovingDirection::EMD_BackwardRight;
		if (bMovingBackwardLeft)  MovingDirection = EMovingDirection::EMD_BackwardLeft;
	}

	if (bOnlyMovingStraightAim) {

		if (bForwardAim)  MovingDirection = EMovingDirection::EMD_ForwardAim;
		if (bBackwardAim) MovingDirection = EMovingDirection::EMD_BackwardAim;
		if (bRightAim)    MovingDirection = EMovingDirection::EMD_RightAim;
		if (bLeftAim)     MovingDirection = EMovingDirection::EMD_LeftAim;
	}

	if (bOnlyMovingDiagonalAim) {

		if (bForwardRightAim)  MovingDirection = EMovingDirection::EMD_ForwardRightAim;
		if (bForwardLeftAim)   MovingDirection = EMovingDirection::EMD_ForwardLeftAim;
		if (bBackwardRightAim) MovingDirection = EMovingDirection::EMD_BackwardRightAim;
		if (bBackwardLeftAim)  MovingDirection = EMovingDirection::EMD_BackwardLeftAim;
	}

	if (bOnlyMovingStraighCrouch) {

		if (bForwardCrouch)  MovingDirection = EMovingDirection::EMD_ForwardCrouch;
		if (bBackwardCrouch) MovingDirection = EMovingDirection::EMD_BackwardCrouch;
		if (bRightCrouch)    MovingDirection = EMovingDirection::EMD_RightCrouch;
		if (bLeftCrouch)     MovingDirection = EMovingDirection::EMD_LeftCrouch;
	}

	if (bOnlyMovingDiagonalyCrouch) {

		if (bForwardRightCrouch)  MovingDirection = EMovingDirection::EMD_ForwardRightCrouch;
		if (bForwardLeftCrouch)   MovingDirection = EMovingDirection::EMD_ForwardLeftCrouch;
		if (bBackwardRightCrouch) MovingDirection = EMovingDirection::EMD_BackwardRightCrouch;
		if (bBackwardLeftCrouch)  MovingDirection = EMovingDirection::EMD_BackwardLeftCrouch;
	}

	if (bOnlyMovingStraighCrouchnAim) {

		if (bForwardCrouchAim)  MovingDirection = EMovingDirection::EMD_ForwardCrouchAim;
		if (bBackwardCrouchAim) MovingDirection = EMovingDirection::EMD_BackwardCrouchAim;
		if (bRightCrouchAim)    MovingDirection = EMovingDirection::EMD_RightCrouchAim;
		if (bLeftCrouchAim)     MovingDirection = EMovingDirection::EMD_LeftCrouchAim;
	}

	if (bOnlyMovingDiagonalyCrouchnAim) {

		if (bForwardRightCrouchAim)  MovingDirection = EMovingDirection::EMD_ForwardRightCrouchAim;
		if (bForwardLeftCrouchAim)   MovingDirection = EMovingDirection::EMD_ForwardLeftCrouchAim;
		if (bBackwardRightCrouchAim) MovingDirection = EMovingDirection::EMD_BackwardRightCrouchAim;
		if (bBackwardLeftCrouchAim)  MovingDirection = EMovingDirection::EMD_BackwardLeftCrouchAim;
	}
	// TODO: finish
	if (bJumping && bOnlyMovingStraightJump) {

		if (bMovingForward && bJumping)  MovingDirection = EMovingDirection::EMD_Jump_Forward;
		if (bMovingBackward && bJumping) MovingDirection = EMovingDirection::EMD_Jump_Backward;
		if (bMovingRight && bJumping)    MovingDirection = EMovingDirection::EMD_Jump_Right;
		if (bMovingLeft && bJumping)     MovingDirection = EMovingDirection::EMD_Jump_Left;
	}
	// clang-format on
}

void AShooterCharacter::SetMovingDirectionActions(float &DeltaTime) {

	if (bDisableCameraLagWhenMoving) return;

	if (!bAiming && !bCrouching) {

		switch (MovingDirection) {

			case EMovingDirection::EMD_Forward :
				return AdjustCameraLag(OffsetFroward, CameraLagMaxDistance_Froward,
				   Froward_InterpTime, DeltaTime, "EMovingDirection::EMD_Forward");

			case EMovingDirection::EMD_Backward :
				return AdjustCameraLag(OffsetBackwards, CameraLagMaxDistance_Backwards,
				   Backwards_InterpTime, DeltaTime, "EMovingDirection::EMD_Backward");

			case EMovingDirection::EMD_Right :
				return AdjustCameraLag(OffsetRight, CameraLagMaxDistance_Right, Right_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_Right");

			case EMovingDirection::EMD_Left :
				return AdjustCameraLag(OffsetLeft, CameraLagMaxDistance_Left, DeltaTime,
				   Left_InterpTime, "EMovingDirection::EMD_Left");

			case EMovingDirection::EMD_ForwardRight :
				return AdjustCameraLag(OffsetForwardRight, CameraLagMaxDistance_ForwardRight,
				   ForwardRight_InterpTime, DeltaTime, "EMovingDirection::EMD_ForwardRight");

			case EMovingDirection::EMD_ForwardLeft :
				return AdjustCameraLag(OffsetForwardLeft, CameraLagMaxDistance_ForwardLeft,
				   ForwardLeft_InterpTime, DeltaTime, "EMovingDirection::EMD_ForwardLeft");

			case EMovingDirection::EMD_BackwardRight :
				return AdjustCameraLag(OffsetBackwardRight, CameraLagMaxDistance_BackwardRight,
				   BackwardRight_InterpTime, DeltaTime, "EMovingDirection::EMD_BackwardRight");

			case EMovingDirection::EMD_BackwardLeft :
				return AdjustCameraLag(OffsetBackwardLeft, CameraLagMaxDistance_BackwardLeft,
				   BackwardLeft_InterpTime, DeltaTime, "EMovingDirection::EMD_BackwardLeft");

			case EMovingDirection::EMD_Jump :
				return AdjustCameraLag(OffsetJump, CameraLagMaxDistance_Jump, DeltaTime,
				   JumpInterpTime, "EMovingDirection::Jump");

			case EMovingDirection::EMD_None :
				return AdjustCameraLag(OffsetIdle, CameraLagMaxDistance_Idle, 5.f, DeltaTime,
				   "EMovingDirection::EMD_None");
		}

	} else if (bAiming && !bCrouching) {

		switch (MovingDirection) {

			case EMovingDirection::EMD_Aim :
				return AdjustCameraLag(OffsetAim, CameraLagMaxDistance_Aim, Aim_interpTime,
				   DeltaTime, "EMovingDirection::EMD_Aim");

			case EMovingDirection::EMD_ForwardAim :
				return AdjustCameraLag(OffsetForwardAim, CameraLagMaxDistance_ForwardAim,
				   ForwardAim_InterpTime, DeltaTime, "EMovingDirection::EMD_ForwardAim");

			case EMovingDirection::EMD_BackwardAim :
				return AdjustCameraLag(OffsetBackwardAim, CameraLagMaxDistance_BackwardAim,
				   BackwardAim_InterpTime, DeltaTime, "EMovingDirection::EMD_BackwardAim");

			case EMovingDirection::EMD_RightAim :
				return AdjustCameraLag(OffsetRightAim, CameraLagMaxDistance_RightAim,
				   RightAim_InterpTime, DeltaTime, "EMovingDirection::EMD_RightAim");

			case EMovingDirection::EMD_LeftAim :
				return AdjustCameraLag(OffsetLeftAim, CameraLagMaxDistance_LeftAim,
				   LeftAim_InterpTime, DeltaTime, "EMovingDirection::EMD_LeftAim");

			case EMovingDirection::EMD_ForwardRightAim :
				return AdjustCameraLag(OffsetForwardRightAim, CameraLagMaxDistance_ForwardRightAim,
				   ForwardRightAim_InterpTime, DeltaTime, "EMovingDirection::EMD_ForwardRightAim");

			case EMovingDirection::EMD_ForwardLeftAim :
				return AdjustCameraLag(OffsetForwardLeftAim, CameraLagMaxDistance_ForwardLeftAim,
				   ForwardLeftAim_InterpTime, DeltaTime, "EMovingDirection::EMD_ForwardLeftAim");

			case EMovingDirection::EMD_BackwardRightAim :
				return AdjustCameraLag(OffsetBackwardRightAim,
				   CameraLagMaxDistance_BackwardRightAim, BackwardRightAim_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_BackwardRightAim");

			case EMovingDirection::EMD_BackwardLeftAim :
				return AdjustCameraLag(OffsetBackwardLeftAim, CameraLagMaxDistance_BackwardLeftAim,
				   BackwardLeftAim_InterpTime, DeltaTime, "EMovingDirection::EMD_BackwardLeftAim");

			case EMovingDirection::EMD_Jump :
				return AdjustCameraLag(OffsetJump, CameraLagMaxDistance_Jump, JumpInterpTime,
				   DeltaTime, "EMovingDirection::Jump");

			case EMovingDirection::EMD_None :
				return AdjustCameraLag(OffsetIdle, CameraLagMaxDistance_Idle, 5.f, DeltaTime,
				   "EMovingDirection::EMD_None");
		}

	} else if (!bAiming && bCrouching) {

		switch (MovingDirection) {

			case EMovingDirection::EMD_Crouch :
				return AdjustCameraLag(OffsetAim, CameraLagMaxDistance_Aim, Aim_interpTime,
				   DeltaTime, "EMovingDirection::EMD_Crouch");

			case EMovingDirection::EMD_ForwardCrouch :
				return AdjustCameraLag(OffsetAim, CameraLagMaxDistance_Aim, ForwardCrouchInterpTime,
				   DeltaTime, "EMovingDirection::EMD_ForwardCrouch");

			case EMovingDirection::EMD_BackwardCrouch :
				return AdjustCameraLag(OffsetBackwardCrouch, CameraLagMaxDistance_BackwardCrouch,
				   BackwardCrouch_InterpTime, DeltaTime, "EMovingDirection::EMD_BackwardCrouch");

			case EMovingDirection::EMD_RightCrouch :
				return AdjustCameraLag(OffsetRightCrouch, CameraLagMaxDistance_RightCrouch,
				   RightCrouch_InterpTime, DeltaTime, "EMovingDirection::EMD_RightCrouch");

			case EMovingDirection::EMD_LeftCrouch :
				return AdjustCameraLag(OffsetLeftCrouch, CameraLagMaxDistance_LeftCrouch,
				   LeftCrouch_InterpTime, DeltaTime, "EMovingDirection::EMD_LeftCrouch");

			case EMovingDirection::EMD_ForwardRightCrouch :
				AdjustCameraLag(OffsetForwardRightCrouch, CameraLagMaxDistance_ForwardRightCrouch,
				   ForwardRightCrouch_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_ForwardRightCrouch");

			case EMovingDirection::EMD_ForwardLeftCrouch :
				return AdjustCameraLag(OffsetLeftCrouch, CameraLagMaxDistance_LeftCrouch,
				   ForwardLeftCrouch_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_ForwardLeftCrouch");

			case EMovingDirection::EMD_BackwardRightCrouch :
				return AdjustCameraLag(OffsetForwardCrouch, CameraLagMaxDistance_ForwardCrouch,
				   BackwardRightCrouch_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_BackwardCrouch");

			case EMovingDirection::EMD_BackwardLeftCrouch :
				AdjustCameraLag(OffsetBackwardRightCrouch, CameraLagMaxDistance_BackwardRightCrouch,
				   BackwardLeftCrouch_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_BackwardLeftCrouch");
		}

	} else if (bAiming && bCrouching) {

		switch (MovingDirection) {

			case EMovingDirection::EMD_CrouchAim :
				return AdjustCameraLag(OffsetAim, CameraLagMaxDistance_Aim, CrouchAim_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_CrouchAim");

			case EMovingDirection::EMD_ForwardCrouchAim :
				return AdjustCameraLag(OffsetForwardCrouchAim,
				   CameraLagMaxDistance_ForwardCrouchAim, ForwardCrouchAim_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_ForwardCrouchAim");

			case EMovingDirection::EMD_BackwardCrouchAim :
				return AdjustCameraLag(OffsetBackwardCrouchAim,
				   CameraLagMaxDistance_BackwardCrouchAim, BackwardCrouchAim_InterpTime, DeltaTime,
				   "EMovingDirection::EMD_BackwardCrouchAim");

			case EMovingDirection::EMD_RightCrouchAim :
				return AdjustCameraLag(OffsetRightCrouchAim, CameraLagMaxDistance_RightCrouchAim,
				   RightCrouchAim_InterpTime, DeltaTime, "EMovingDirection::EMD_RightCrouchAim");

			case EMovingDirection::EMD_LeftCrouchAim :
				return AdjustCameraLag(OffsetLeftCrouchAim, CameraLagMaxDistance_LeftCrouchAim,
				   LeftCrouchAim_InterpTime, DeltaTime, "EMovingDirection::EMD_LeftCrouchAim");

			case EMovingDirection::EMD_ForwardRightCrouchAim :
				return AdjustCameraLag(OffsetForwardRightCrouchAim,
				   CameraLagMaxDistance_ForwardRightCrouchAim, ForwardRightCrouchAim_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_ForwardRightCrouchAim");

			case EMovingDirection::EMD_ForwardLeftCrouchAim :
				return AdjustCameraLag(OffsetForwardLeftCrouchAim,
				   CameraLagMaxDistance_ForwardLeftCrouchAim, ForwardLeftCrouchAim_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_ForwardLeftCrouchAim");

			case EMovingDirection::EMD_BackwardRightCrouchAim :
				return AdjustCameraLag(OffsetForwardCrouchAim,
				   CameraLagMaxDistance_ForwardCrouchAim, BackwardRightCrouchAim_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_BackwardCrouch");

			case EMovingDirection::EMD_BackwardLeftCrouchAim :
				return AdjustCameraLag(OffsetBackwardRightCrouchAim,
				   CameraLagMaxDistance_BackwardRightCrouchAim, BackwardLeftCrouchAim_InterpTime,
				   DeltaTime, "EMovingDirection::EMD_BackwardLeftCrouchAim");

			case EMovingDirection::EMD_Jump :
				return AdjustCameraLag(OffsetJump, CameraLagMaxDistance_Jump, DeltaTime,
				   JumpInterpTime, "EMovingDirection::Jump");

			case EMovingDirection::EMD_None :
				return AdjustCameraLag(OffsetIdle, CameraLagMaxDistance_Idle, 5.f, DeltaTime,
				   "EMovingDirection::EMD_None");
		}

	} else if (bIsJumping) {

		return AdjustCameraLag(OffsetJump, CameraLagMaxDistance_Jump, DeltaTime, JumpInterpTime,
		   "EMovingDirection::Jump");
	}
}

void AShooterCharacter::EMovingDirection_None(float DeltaTime) {

	CustomCameraLagMaxDistance = OriginalCameraLagMaxDistance;
	CustomCameraSocketOffset   = OriginalCameraSocketOffset;

	const double  GetInterpMaxDistance {FMath::FInterpTo(
		CameraBoom->CameraLagMaxDistance, CustomCameraLagMaxDistance, DeltaTime, 5.f)};

	const FVector GetInterpSocket {FMath::VInterpTo(CameraBoom->SocketOffset,
	   OriginalCameraSocketOffset, DeltaTime, 5.f)};

	CameraBoom->CameraLagMaxDistance = GetInterpMaxDistance;
	CameraBoom->SocketOffset		 = GetInterpSocket;
}

void AShooterCharacter::AdjustCameraLag(const FVector &Offset, const double &CameraLagMaxDistance,
   const float &InterpTime, float &DeltaTime, const char *DebugMessage) {

	if (bShowEMovingDirection) PrintOnScrColorTime(-1, FColor::Emerald, DebugMessage);

	if (Offset == FVector::ZeroVector) return AShooterCharacter::EMovingDirection_None(DeltaTime);

	FVector CustomSocket {Offset};

	if (CustomSocket.X == 0) CustomSocket.X = CameraBoom->SocketOffset.X;
	if (CustomSocket.Y == 0) CustomSocket.Y = CameraBoom->SocketOffset.Y;
	if (CustomSocket.Z == 0) CustomSocket.Z = CameraBoom->SocketOffset.Z;

	CustomCameraSocketOffset = CustomSocket;

	const double  GetInterpMaxDistance {FMath::FInterpTo(
		CameraBoom->CameraLagMaxDistance, CameraLagMaxDistance, DeltaTime, InterpTime)};

	const FVector GetInterpSocket {FMath::VInterpTo(CameraBoom->SocketOffset,
	   CustomCameraSocketOffset, DeltaTime, InterpTime)};

	CameraBoom->CameraLagMaxDistance = GetInterpMaxDistance;
	CameraBoom->SocketOffset		 = GetInterpSocket;
}

EPhysicalSurface AShooterCharacter::GetSurfaceType() {

	FHitResult			  HitResult		= {};
	const FVector		  Start			= {GetActorLocation()};
	const FVector		  Bellow		= {FVector(0.f, 0.f, -400.f)};
	const FVector		  End			= {Start + Bellow};
	FCollisionQueryParams QueryParams	= {};

	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,
	   ECollisionChannel::ECC_Visibility, QueryParams);

	return UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
}
