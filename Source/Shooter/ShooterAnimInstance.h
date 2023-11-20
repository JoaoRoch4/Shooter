#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"
#include "Custom.h"
#include "ShooterAnimInstance.generated.h"

using namespace UnrealBasic;

UENUM(BlueprintType)
enum class EOffsetState : uint8 {

    EOS_Aiming    UMETA(DisplayName = "Aiming"),
    EOS_Hip       UMETA(DisplayName = "Hip"),
    EOS_Reloading UMETA(DisplayName = "Reloading"),
    EOS_InAir     UMETA(DisplayName = "InAir"),

    EOS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance {

    GENERATED_BODY()

public:
    UShooterAnimInstance();

    UFUNCTION(BlueprintCallable)
    void UpdateAnimationProperties(float DeltaTime);

    virtual void NativeInitializeAnimation() override;

    /**
	 * @brief Handles Calculations for leaning while running.

     * * @param DeltaTime Gets DeltaTime from the
	 *
     * UpdateAnimationProperties function.
	 */
    void Lean(float DeltaTime);

protected:
    /**
	 * @brief Handle turn in place variables.
	 */
    void TurnInPlace();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    class AShooterCharacter *ShooterCharacter;

    /** The Speed of the Character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float Speed;

    /** Whether or not the character is in the air */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsInAir;

    /** Whether or not the character is moving */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bIsAccelerating;

    /** Offset yaw use for strafing */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float MovementOffsetYaw;

    /** OffsetYaw the frame before we stopped moving */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float LastMovementOffsetYaw;

    /**  true if the Character is aiming. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    bool bAiming;

    /** 
	 * @brief Yaw of the character this frame; Only updated

     * *  when Standin still and not in air.
	 */
    float TIPCharacterYaw;

    /*
	 * @brief Yaw of the character the previous frame; Only updated

     * *  when Standin still and not in air. 
	 */
    float TIPCharacterYawLastFrame;

    /**  Offset of the Yaw. */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
    float RootYawOffset;

    /**  Rotation Curve Value this frame. */
    float RotationCurve;

    /**  Rotation Curve Value last frame. */
    float RotationCurveLastFrame;

    /**  The pitch of the aim rotation, used for Aim Offset. */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
    float Pitch;

    /**
	 * @brief True when reloading, used to prevent
	 * Aim Offset
     * while reloading. 
	 */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
    bool bReloading;

    /**
	 * @brief Offset state; used to determine which Aim
	 *
     * Offset to use. 
	 */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadOnly, Category = "Turn In Place", meta = (AllowPrivateAccess = "true"))
    EOffsetState OffsetState;

    /**  Character Yaw this frame. */
    FRotator CharacterRotation;

    /**  Character Yaw last frame. */
    FRotator CharacterRotationLastFrame;

    /**
	 * @brief Yaw delta used for leaning in the 
	 * running
     * BlendSpace. 
	 */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lean", meta = (AllowPrivateAccess = "true"))
    float YawDelta;

    /**  True when crouching. */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadWrite, Category = "Crouching", meta = (AllowPrivateAccess = "true"))
    bool bCrouching;

     /**  True when Equipping. */
    UPROPERTY(
      VisibleAnywhere, BlueprintReadWrite, Category = "Equipping", meta = (AllowPrivateAccess = "true"))
    bool bEquipping;

    /**
	   Change the recoil weight based on turning
	  * in
     * place and aiming. 
	  */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    float RecoilWeight;

    /**  True when turning in place. */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    bool bTurningInPlace;
};
