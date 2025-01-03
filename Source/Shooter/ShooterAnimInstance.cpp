#include "ShooterAnimInstance.h"

#include "Custom.h"
#include "ShooterCharacter.h"
#include "Weapon.h"
#include "WeaponType.h"

#include <GameFramework\CharacterMovementComponent.h>
#include <Kismet\KismetMathLibrary.h>
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathUtility.h"
#include "Templates/Casts.h"

UShooterAnimInstance::UShooterAnimInstance()
 : ShooterCharacter(nullptr)
 , Speed(0)
 , bIsInAir(false)
 , bIsAccelerating(false)
 , MovementOffsetYaw(0.0f)
 , LastMovementOffsetYaw(0.0f)
 , bAiming(false)
 , TIPCharacterYaw(0.0f)
 , TIPCharacterYawLastFrame(0.0f)
 , RootYawOffset(0.0f)
 , RotationCurve(0.0f)
 , RotationCurveLastFrame(0.0f)
 , Pitch(0.0f)
 , bReloading(false)
 , OffsetState(EOffsetState::EOS_Hip)
 , CharacterRotation(FRotator())
 , CharacterRotationLastFrame(FRotator())
 , YawDelta(0.0f)
 , bCrouching(false)
 , bEquipping(false)
 , RecoilWeight(1.f)
 , bTurningInPlace(false)
 , WeaponType(EWeaponType::EWT_Pistol)
 , bShouldUseFABRIK(false) {}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime) {

    if (ShooterCharacter == nullptr) ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());

    if (ShooterCharacter == nullptr) return;

    bCrouching = ShooterCharacter->GetCrouching();
    bReloading = ShooterCharacter->GetCombatState() == ECombatState::ECS_Reloading;
    bEquipping = ShooterCharacter->GetCombatState() == ECombatState::ECS_Equipping;

    bShouldUseFABRIK = ShooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied
                    || ShooterCharacter->GetCombatState() == ECombatState::ECS_FireTimerInProgress;

    // Get the lateral speed of the character from velocity
    FVector Velocity {ShooterCharacter->GetVelocity()};
    Velocity.Z = 0;
    Speed      = Velocity.Size();

    // Is the character in the air?
    bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

    bIsAccelerating
      = (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) ? true : false;

    const FRotator AimRotation {ShooterCharacter->GetBaseAimRotation()};
    const FRotator MovementRotation {UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity())};

    MovementOffsetYaw
      = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

    if (ShooterCharacter->GetVelocity().Size() > 0.f) LastMovementOffsetYaw = MovementOffsetYaw;

    bAiming = ShooterCharacter->GetAiming();

    if (bReloading) 
        OffsetState = EOffsetState::EOS_Reloading;
    else if (bIsInAir) 
        OffsetState = EOffsetState::EOS_InAir;
    else if (ShooterCharacter->GetAiming()) 
        OffsetState = EOffsetState::EOS_Aiming;
    else 
        OffsetState = EOffsetState::EOS_Hip;
    
    // Check if ShooterCharacter has a valid EquippedWeapon
    if (ShooterCharacter->GetEquippedWeapon())
        WeaponType = ShooterCharacter->GetEquippedWeapon()->GetWeaponType();
    
    TurnInPlace();
    Lean(DeltaTime);
}

void UShooterAnimInstance::NativeInitializeAnimation() {
    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::TurnInPlace() {

    if (ShooterCharacter == nullptr) return;

    Pitch = ShooterCharacter->GetBaseAimRotation().Pitch;

    if (Speed > 0 || bIsInAir) {

        // Don't want to turn in place; Character is moving
        RootYawOffset            = 0.f;
        TIPCharacterYaw          = ShooterCharacter->GetActorRotation().Yaw;
        TIPCharacterYawLastFrame = TIPCharacterYaw;
        RotationCurveLastFrame   = 0.f;
        RotationCurve            = 0.f;

    } else {

        TIPCharacterYawLastFrame = TIPCharacterYaw;
        TIPCharacterYaw          = ShooterCharacter->GetActorRotation().Yaw;
        const float TIPYawDelta {TIPCharacterYaw - TIPCharacterYawLastFrame};

        // Root Yaw Offset, updated and clamped to [-180, 180]
        RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

        // 1.0 if turning, 0.0 if not
        const float Turning {GetCurveValue(L"Turning")};

        if (Turning > 0) {

            bTurningInPlace        = true;
            RotationCurveLastFrame = RotationCurve;
            RotationCurve          = GetCurveValue(L"Rotation");
            const float DeltaRotation {RotationCurve - RotationCurveLastFrame};

            // RootYawOffset > 0, -> Turning Left. RootYawOffset < 0, -> Turning Right.
            (RootYawOffset > 0) ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

            const float ABSRootYawOffset {FMath::Abs(RootYawOffset)};

            if (ABSRootYawOffset > 90.f) {

                const float YawExcess {ABSRootYawOffset - 90.f};
                (RootYawOffset > 0) ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
            }
        } else {
            bTurningInPlace = false;
        }
    }

    // Set the Recoil Weight
    if (bTurningInPlace) {
        if (bReloading || bEquipping) {
            RecoilWeight = 1.f;
        } else {
            RecoilWeight = 0.f;
        }
    } else { // not turning in place
    
        if (bCrouching) {
            if (bReloading || bEquipping) 
                RecoilWeight = 1.f;
            else 
                RecoilWeight = 0.1f;
            
        } else {
            if (bAiming || bReloading || bEquipping) 
                RecoilWeight = 1.f;
            else 
                RecoilWeight = 0.5f;            
        }
    }
}

void UShooterAnimInstance::Lean(float DeltaTime) {

    if (ShooterCharacter == nullptr) return;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation          = ShooterCharacter->GetActorRotation();

    const FRotator Delta {
      UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame)};

    const float DeltaYaw {static_cast<const float>(Delta.Yaw)};

    const float Target {DeltaYaw / DeltaTime};

    const float Interp {FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f)};

    YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}