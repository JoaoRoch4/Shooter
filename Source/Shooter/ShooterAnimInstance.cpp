#include "ShooterAnimInstance.h"

#include "Custom.h"
#include "ShooterCharacter.h"
#include "Weapon.h"
#include "WeaponType.h"

#include <GameFramework\CharacterMovementComponent.h>
#include <Kismet\KismetMathLibrary.h>

UShooterAnimInstance::UShooterAnimInstance()
 : ShooterCharacter(nullptr)
 , Speed(0.0f)
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
 , WeaponType(EWeaponType::EWT_MAX)
 , bShouldUseFABRIK(false) {}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime) {

    if (ShooterCharacter == nullptr) ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());

    if (ShooterCharacter) {

        bCrouching = ShooterCharacter->GetCrouching();
        bReloading = ShooterCharacter->GetCombatState() == ECombatState::ECS_Reloading;
        bEquipping = ShooterCharacter->GetCombatState() == ECombatState::ECS_Equipping;

        bool Unoccupied {ShooterCharacter->GetCombatState() == ECombatState::ECS_Unoccupied};
        bool FireTimer  {ShooterCharacter->GetCombatState() == ECombatState::ECS_FireTimerInProgress};
        bShouldUseFABRIK = Unoccupied || FireTimer;

        // Get the lateral speed of the character from velocity
        FVector Velocity {ShooterCharacter->GetVelocity()};

        Velocity.Z = 0;

        Speed = Velocity.Size();

        // Is the character in the air?
        bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

        if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)

            bIsAccelerating = true;

        else bIsAccelerating = false;

        FRotator AimRotation {ShooterCharacter->GetBaseAimRotation()};

        FRotator MovementRotation {
          UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity())};

        MovementOffsetYaw
          = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

        if (ShooterCharacter->GetVelocity().Size() > 0.0f)
            LastMovementOffsetYaw = MovementOffsetYaw;

        bAiming = ShooterCharacter->GetAiming();

        if (bReloading) {

            OffsetState = EOffsetState::EOS_Reloading;

        } else if (bIsInAir) {

            OffsetState = EOffsetState::EOS_InAir;

        } else if (ShooterCharacter->GetAiming()) {

            OffsetState = EOffsetState::EOS_Aiming;

        } else {

            OffsetState = EOffsetState::EOS_Hip;
        }

        // Check if ShooterCharacter has a valid equipped weapon
        if (ShooterCharacter->GetEquippedWeapon()) {

            WeaponType = ShooterCharacter->GetEquippedWeapon()->GetWeaponType();

        } else {

            WeaponType = EWeaponType::EWT_SubmachineGun;
        }
    }

    TurnInPlace();
    Lean(DeltaTime);
}

void UShooterAnimInstance::NativeInitializeAnimation() {

    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}

void UShooterAnimInstance::Lean(float DeltaTime) {

    if (ShooterCharacter == nullptr) return;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation          = ShooterCharacter->GetActorRotation();

    const FRotator Delta {
      UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame)};

    const float DeltaYawFloat {StaticCast<float>(Delta.Yaw)};
    const float Target {DeltaYawFloat / DeltaTime};
    const float Interp {FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f)};

    YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}

void UShooterAnimInstance::TurnInPlace() {

    if (ShooterCharacter == nullptr) return;

    Pitch = ShooterCharacter->GetBaseAimRotation().Pitch;

    if (Speed > 0 || bIsInAir) {

        // Don't turn in place if character is moving

        RootYawOffset            = 0.0f;
        TIPCharacterYaw          = ShooterCharacter->GetActorRotation().Yaw;
        TIPCharacterYawLastFrame = TIPCharacterYaw;
        RotationCurveLastFrame   = 0.0f;
        RotationCurve            = 0.0f;

    } else {

        TIPCharacterYawLastFrame = TIPCharacterYaw;
        TIPCharacterYaw          = ShooterCharacter->GetActorRotation().Yaw;
        const float TIPYawDelta {TIPCharacterYaw - TIPCharacterYawLastFrame};

        // Root yaw offset, updated and clamped to [-180, 180]
        RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

        // 1.0f if turning. 0.f if not.
        const float Turning {GetCurveValue(L"Turning")};

        if (Turning > 0) {

            bTurningInPlace        = true;
            RotationCurveLastFrame = RotationCurve;
            RotationCurve          = GetCurveValue(L"Rotation");
            const float DeltaRotation {RotationCurve - RotationCurveLastFrame};

            RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

            const float ABSRootYawOffset {FMath::Abs(RootYawOffset)};

            if (ABSRootYawOffset > 90.0f) {

                const float YawExcess {ABSRootYawOffset - 90.f};

                RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
            }
        } else {

            bTurningInPlace = false;
        }
    }

    // Set the recoil weight
    if (bTurningInPlace) {

        if (bReloading || bEquipping) {

            RecoilWeight = 1.f;

        } else {

            RecoilWeight = 0.0f;
        }

    } else { // not turning in place

        if (bCrouching) {

            if (bReloading || bEquipping) {

                RecoilWeight = 1.f;

            } else {

                RecoilWeight = 0.1f;
            }

        } else {

            if (bAiming || bReloading || bEquipping) {

                RecoilWeight = 1.f;

            } else {

                RecoilWeight = 0.5f;
            }
        }
    }
}
