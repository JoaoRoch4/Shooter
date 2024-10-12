/*****************************************************************/ /**
                                                                     * \file   ShooterCharacter.h
                                                                     * \brief  Main Character Class
                                                                     *
                                                                     * \author João Rocha
                                                                     * \date   December 2023 -
                                                                     *January 2024
                                                                     *********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "AmmoType.h"
#include "Custom.h"
#include "MovingDirection.h"

#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8 {

    ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),

    ECS_FireTimerInProgress UMETA(DisplayName = "FireTimer In Progress"),

    ECS_Reloading UMETA(DisplayName = "Reloading"),

    ECS_Equipping UMETA(DisplayName = "Equipping"),

    ECS_MAX UMETA(DisplayName = "Default MAX")
};

USTRUCT(BlueprintType)
struct FInterpLocation {

    GENERATED_BODY()

    /**  Scene component to use for its location for interping. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent *SceneComponent;

    /**  Number of items interping to / at this scene com location.. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ItemCount;

    FInterpLocation()
     :

     SceneComponent(nullptr)
     , ItemCount(0) {}

    FInterpLocation(USceneComponent *SceneComponent, int32 ItemCount)
     :

     SceneComponent(SceneComponent)
     , ItemCount(ItemCount) {}
};

/*
 *
 */
UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter {

    GENERATED_BODY()

public:

    // Sets default values for this character's properties
    AShooterCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
      FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
      FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** @brief Set Camera Boom Defaults */
    void DefaultConstructor_CameraBoom();

    /** @brief Set Follow Camera Defaults */
    void DefaultConstructor_FollowCamera();

    /** @brief Set Player Mesh Defaults */
    void DefaultConstructor_SetupMesh();

    /** @brief Set Cues for Combat */
    void DefaultConstructor_SetCombatCues();

    /** @brief Set Character Defaults */
    void DefaultConstructor_SetCharacter();

    void DefaultConstructor_CustomCamera();

    /** @brief Create Interp Components */
    void DefaultConstructor_InterpComponents();

    /** @brief Called for forwards / backwards input */
    void MoveForward(float Value);

    /** @brief Called for side to side input */
    void MoveRight(float Value);

    /**
     * @brief Called via input to turn at given rate.
     *
     * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired
     * turn rate.
     */
    void TurnAtRate(float Rate);

    /**
     * @brief Called via input to look up/down a given rate

     * * @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired
     * turn rate.
     */
    void LookUpAtRate(float Rate);

    /**
     * @brief Rotate controller based on mouse X movement
     *
     * @param Value The input value from mouse movement
     */
    void Turn(float Value);

    /**
     * @brief Rotate controller based on mouse Y movement
     *
     * @param Value The input value from mouse movement
     */
    void LookUp(float Value);

    bool IsMovingRight();

    void DisableCameraLagWhenMovingRight(float DeltaTime);

    void DisableCameraLagWhenMovingBackwards(float DeltaTime);

    /** @brief Called when the fire button is pressed */
    void FireWeapon();

    /** @brief Modify a FVector to get the location of bean end */
    bool GetBeanEndLocation(const FVector &MuzzleSocketLocation, FVector &OutBeanLocation);

    void Relative_ControllerRotationYaw(float DeltaTime);

    void SetupFollowCamera();

    /** @brief Starts CameraLerp with DeltaTime */
    void StartCameraLerp(float &DeltaTime);

    /** @brief Turn on Cinematic Camera */
    void CinematicCameraOn();

    /** @brief Turn off Cinematic Camera */
    void CinematicCameraOff();

    /** @brief When called activate\deactivate Cinematic Camera */
    void ToggleCinematicCamera();

    /** @brief Interpolation for aiming zoom */
    void AimingCameraZoom(float DeltaTime);

    float BezierCurve_Interp(const float &CurrentFOV, const float &TargetFOV, float DeltaTime,
      float InterpSpeed, bool bReturnToOriginal);

    /** Set BaseTurnRate and BaseLookUpRate based on aiming */
    void SetsLookRates();

    void Toggle_bIsFiringWeapon_On();
    void Toggle_bIsFiringWeapon_Off();

    /** @brief Set bAiming to true or false with button press */
    void AimingButtonPressed();
    void AimingButtonReleased();

    void CalculateCrosshairSpread(const float &DeltaTime);

    void StartCrosshairBulletFire();

    UFUNCTION()
    void FinishCrosshairBulletFire();

    void FireButtonPressed();
    void FireButtonReleased();

    void StartFireTimer();

    UFUNCTION()
    void AutoFireReset();

    /**
     * @brief Line trace for itens under the crosshair.
     *
     * @param OutHitResult First blocking hit found.
     * @param OutHitLocation
     * if hit, the location of the hit.
     * @return true if hit
     * something. And false if not.
     */
    bool TraceUnderCrosshairs(FHitResult &OutHitResult, FVector &OutHitlocation);

    /**
     * @brief Trace for itens under the crosshair if Overlapped
     * item
     *  count is > 0.
     */
    void TraceForItems();

    /** @brief Spawns a default Weapon and equip it */
    class AWeapon *SpawnDefaultWeapon();

    /**
     * @brief Takes a weapon and attaches to the mesh
     *
     * @param WeaponToEquip The weapon to equip
     */
    void EquipWeapon(AWeapon *WeaponToEquip, bool bSwapping = false);

    /**
     * @brief Detaches weapon, let it fall to the ground and sets
     * equipped
     *  weapon to null
     */
    void DropWeapon();

    void SelectButtonPressed();
    void SelectButtonReleased();

    /**
     * @brief Drops currently equipped weapon and equips
     * TraceHitItem
     * @param WeaponToSwap The weapon to equip
     */
    void SwapWeapon(AWeapon *WeaponToSwap);

    /**
     * @brief Initialize the Ammo Map with Ammo values.
     */
    void InitializeAmmoMap();

    /**
     * @brief Check to make sure if our weapon has ammo.
     *
     * @return True if has ammo
     */
    bool WeaponHasAmmo();

    /** Fire weapon functions */
    void PlayFireSound();
    void SendBullet();
    void PlayGunFireMontage();

    /**
     * @brief Bound to the R key and gamepad face button left.

     */
    void ReloadButtonPressed();

    /**
     * @brief Handle the reloading of Weapon.
     */
    void ReloadWeapon();

    UFUNCTION(BlueprintCallable)
    void FinishReloading();

    UFUNCTION(BlueprintCallable)
    void FinishEquipping();

    /**
     * @brief Check if we have ammo of the Equipped Weapons

     * * ammo type.
     * @return true if has ammo
     */
    bool CarryingAmmo();

    /**
     * @brief Called from Animation Blueprint with GrabClip notify.

     */
    UFUNCTION(BlueprintCallable)
    void GrabClip();

    /**
     * @brief Called from Animation Blueprint with ReleaseClip
     * notify.
     */
    UFUNCTION(BlueprintCallable)
    void ReleaseClip();

    void CrouchButtonPressed();
    virtual void Jump() override;
    virtual void StopJumping() override;

    /**
     * @brief Interps capsule half height when crouching/standing.
     *
     * * @param DeltaTime Called every frame
     */
    void InterpCapsuleHalfHeight(float DeltaTime);

    void Aim();
    void StopAiming();
    void PickupAmmo(class AAmmo *Ammo);
    void InitializeInterpLocations();
    FORCEINLINE void ResetPickupSoundTimer();
    FORCEINLINE void ResetEquipSoundTimer();
    void ExchangeInventoryItems(int32 CurrentItemindex, int32 NewItemIndex);
    void PreviousSlot();

    UFUNCTION(BlueprintCallable)
    void EnableExchangeInventoryItems();

    void DebugSlotsItems();
    void UpdateSlotsItems();
    void EquipWeapon();
    int32 GetEmptyInventorySlot();
    void HighlightInventorySlot();
    void HandleMouseWheel(float Value);
    void ScrollUp();
    void ScrollDown();

    /* Key Bindings */
    void KEY_FkeyPressed();

    void KEY_1_OneKeyPressed();
    void KEY_2_TwoKeyPressed();
    void KEY_3_ThreeKeyPressed();
    void KEY_4_FourKeyPressed();
    void KEY_5_FiveKeyPressed();
    void KEY_6_SixKeyPressed();
    void KEY_7_SevenKeyPressed();
    void KEY_8_EightKeyPressed();
    void KEY_9_NineKeyPressed();
    void KEY_0_ZeroKeyPressed();

    void KEY_DKey_D_Pressed();
    void KEY_DKey_D_Released();

    void KEY_SKey_S_Pressed();
    void KEY_SKey_S_Released();

    void KEY_WKey_W_Pressed();
    void KEY_WKey_W_Released();

    void KEY_AKey_A_Pressed();
    void KEY_AKey_A_Released();

    /* Key Methods */
    void KeyMethodFKey();

    void KeyMethodDKey();
    void KeyMethodDKeyReleased();

    void KeyMethodSKey();
    void KeyMethodSKeyReleased();

    void KeyMethodWKey();
    void KeyMethodWKeyReleased();

    void KeyMethodAKey();
    void KeyMethodAKeyReleased();

    void KeyMethod1Key();
    void KeyMethod2Key();
    void KeyMethod3Key();
    void KeyMethod4Key();
    void KeyMethod5Key();
    void KeyMethod6Key();
    void KeyMethod7Key();
    void KeyMethod8Key();
    void KeyMethod9Key();
    void KeyMethod0Key();

    void SetMovingLogic();
    void SetMovingDirection();
    void SetMovingDirectionActions(float &DeltaTime);

    UFUNCTION(BlueprintCallable)
    void EMovingDirection_None(float DeltaTime);

    void GetOriginalCameraLagOffset();
    void AdjustVectors();
    void AdjustCameraLag(const FVector &Offset, const double &CameraLagMaxDistance,
      const float &InterpTime, float &DeltaTime, const char *DebugMessage);

    UFUNCTION(BlueprintCallable)
    EPhysicalSurface GetSurfaceType();

private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Mesh",
      meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent *PlayerMesh;

    class TUniquePtr<struct ConstructorHelpers::FObjectFinder<USkeletalMesh>> SkeletalMeshContainer;

    /* Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = Category = "My Custom Properties|My Custom Camera|",
      meta                = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

    /* Camera that follows the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = Category = "My Custom Properties|My Custom Camera|",
      meta                = (AllowPrivateAccess = "true"))
    class UCameraComponent *FollowCamera;

    /* Camera that follows the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = Category = "My Custom Properties|My Custom Camera|",
      meta                = (AllowPrivateAccess = "true"))
    class UShooterCharacterCamera *CustomCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = Category = "My Custom Properties|My Custom Camera|",
      meta                = (AllowPrivateAccess = "true"))
    bool bUseCustomCamera;

    /**
     * @brief Is transitioning for cinematic camera to normal
     * camera
     *  or vice versa.
     */
    UPROPERTY()
    bool bIsTransitioning;

    UPROPERTY()
    bool bDKey_Pressed;
    UPROPERTY()
    bool bDKey_Released;

    UPROPERTY()
    bool bSKey_Pressed;
    UPROPERTY()
    bool bSKey_Released;

    UPROPERTY()
    bool bWKey_Pressed;
    UPROPERTY()
    bool bWKey_Released;

    UPROPERTY()
    bool bAKey_Pressed;
    UPROPERTY()
    bool bAKey_Released;

    /** Is true for Cinematic camera */
    UPROPERTY()
    bool bCinematicCameraSwitch;

    /** Use bezier curve for Cinematic camera */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    bool bUseBezierCurve;

    /** Camera Arm Length Start Editable on editor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float CameraArmLengthStart;

    /** Camera Arm Length End Editable on editor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float CameraArmLengthEnd;

    /** Duration of the transition */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float TransitionDuration;

    UPROPERTY(BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float PreviousYaw;

    UPROPERTY(BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float LeapedArmLength;

    /** Current time of the transition */
    UPROPERTY(BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Cinematic Camera",
      meta     = (AllowPrivateAccess = "true"))
    float CurrentTime;

    /** Base turn rate, in deg / sec. Other scaling may affect final turn rate
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final turn
     * rate */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float BaseLookUpRate;

    /** Turn rate while not aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float HipTurnRate;

    /** Look up rate while not aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float HipLookUpRate;

    /** Turn rate while aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float AimingTurnRate;

    /**  Look up rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess))
    float AimingLookUpRate;

    /**  Mouse turn rate while not aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess),
      meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseHipTurnRate;

    /**  Mouse look up rate while not aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess),
      meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseHipLookUpRate;

    /**  Mouse turn rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess),
      meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseAimingTurnRate;

    /**  Mouse look up rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess),
      meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseAimingLookUpRate;

    /**  true if is firing the weapon. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Combat|Fire",
      meta = (AllowPrivateAccess = "true"))
    bool bIsFiringWeapon;

    /**  true if is firing the weapon. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Combat|Fire",
      meta = (AllowPrivateAccess = "true"))
    bool bDidFire;

    /**  Particles spawned upon bullet impact. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UParticleSystem *ImpactParticles;

    /**  Smoke trail for bullets. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UParticleSystem *BeamParticles;

    /**  Montage for firing the weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    class UAnimMontage *HipFireMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UAnimMontage *EquipMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Debug",
      meta = (AllowPrivateAccess = "true"))
    bool bShowCustomDebugMessages;

    /** True when Aiming */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess = "true"))
    bool bAiming;

    /**  Default camera field of view value. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess = "true"))
    float CameraDefaultAimFOV;

    /**  Field of view value for when zoomed in. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess = "true"))
    float CameraZoomedAimFOV;

    /**  Current field of view this frame. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|My Custom Camera|Aim",
      meta = (AllowPrivateAccess = "true"))
    float CameraCurrentAimFOV;

    /**  Interp speed for zooming when aiming. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Aim", meta = (AllowPrivateAccess = "true"))
    float ZoomInterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairHeight;

    /**  Determines the spread of the crosshairs. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float CrosshairSpreadMultiplier;

    /**  Velocity component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float CrosshairVelocityFactor;

    /**  In air component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float CrosshairInAirFactor;

    /**  Aim component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float CrosshairAimFactor;

    /**  Shooting component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float CrosshairShootingFactor;

    float ShootTimeDuration;

    bool bFiringBullet;

    FTimerHandle CrosshairShootTimer;

    /**  Left mouse button or right console trigger pressed. */
    bool bFireButtonPressed;

    /**  True when we can fire. False when waiting for the timer. */
    bool bShouldFire;

    /**  Rate of automatic gun fire. */
    float AutomaticFireRate;

    /**  Sets a timer between gun shots. */
    FTimerHandle AutoFireTimer;

    /**
     * @brief True when we can trace a line for items.
     */
    bool bShouldTraceForItems;

    /**
     * @brief Number of overlapped AItens.
     */
    int8 OverlappedItemCount;

    /**
     * @brief Last AItem traced.
     */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Combat|Class", meta = (AllowPrivateAccess = "true"))
    class AItem *TraceHitItemLastFrame;

    /**
     * @brief Currently equipped weapon.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    class AWeapon *EquippedWeapon;

    /**
     * @brief Sets this in Blueprints for the default weapon class.

     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    TSubclassOf<AWeapon> DefaultWeaponClass;

    /**
     * @brief Item currently hit by trace in TraceForItens.

     * * (Could be null)
     */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    AItem *TraceHitItem;

    /**
     * @brief Distance outward from the camera for the
     *
     * interp destination.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Itens|Camera",
      meta = (AllowPrivateAccess = "true"))
    float CameraInterpDistance;

    /**
     * @brief Distance Upward from the camera for the
     * interp
     * destination.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Itens|Camera",
      meta = (AllowPrivateAccess = "true"))
    float CameraInterpElevation;

    /**  Map to keep track of ammo of different types. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Items|Ammo",
      meta = (AllowPrivateAccess = "true"))
    TMap<EAmmoType, int32> AmmoMap;

    /** @brief Starting 9mm ammo. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Items|Ammo",
      meta = (AllowPrivateAccess = "true"))
    int32 Starting_9mm_Ammo;

    /** @brief Starting AR ammo. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Items|Ammo",
      meta = (AllowPrivateAccess = "true"))
    int32 Starting_AR_Ammo;

    /**  Combat State. Cannot fire or reload when unoccupied. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat",
      meta = (AllowPrivateAccess = "true"))
    ECombatState CombatState;

    /** @brief Montage for reloading animations. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UAnimMontage *ReloadMontage;

    /**  Transform of the clip when we first grab the clip during
     * reloading. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Animation",
      meta = (AllowPrivateAccess = "true"))
    FTransform ClipTransform;

    /**  Scene component to attach to the Character's hand during
     * reloading. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Animation",
      meta = (AllowPrivateAccess = "true"))
    USceneComponent *HandSceneComponent;

    /** @brief True when crouching. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    bool bCrouching;

    /**  Regular Movement speed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float BaseMovementSpeed;

    /**  Crouch Movement Speed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchMovementSpeed;

    /**  Current Half Height of the Capsule. */
    float CurrentCapsuleHalfHeight;

    /**  Half Height of the Capsule when not Crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float StandingCapsuleHalfHeight;

    /**  Half Height of the Capsule when Crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchingCapsuleHalfHeight;

    /**  Ground Friction while not crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float BaseGroundFriction;

    /**  Ground Friction while crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchingGroundFriction;

    /**  Used for knowing the Aiming Button is Pressed. */
    bool bAimingButtonPressed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *WeaponInterpComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp2;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp5;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    USceneComponent *InterpComp6;

    /**  Array of interp location structs. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    TArray<FInterpLocation> InterpLocations;

    FTimerHandle PickupSoundTimer;
    FTimerHandle EquipSoundTimer;

    bool bShouldPlayPickupSound;
    bool bShouldPlayEquipSound;

    /**  Time to wait before we can play another Pickup Sound. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Items|Sound",
      meta = (AllowPrivateAccess = "true"))
    float PickupSoundResetTime;

    /** Time to wait before we can play another Equip Sound */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Items|Sound",
      meta = (AllowPrivateAccess = "true"))
    float EquipSoundResetTime;

    /** An array of AItems for our Inventory */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Items|Inventory",
      meta = (AllowPrivateAccess = "true"))
    TArray<AItem *> Inventory;

    const int32 InventoryCapacity {6};

    /** Delegate for sending slot information to InventoryBar when equipping */
    UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = "true"))
    FEquipItemDelegate EquipItemDelegate;

    /** Delegate for sending slot information for playing the icon animation */
    UPROPERTY(BlueprintAssignable, Category = "Delegates", meta = (AllowPrivateAccess = "true"))
    FHighlightIconDelegate HighlightIconDelegate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Debug",
      meta = (AllowPrivateAccess = "true"))
    float DebugKeys;

    /** EquippedWeapon->GetSlotIndex() */
    int32 CurrentSlotIndex;

    /** Inventory.Num() */
    int32 InventoryCount;

    int32 LastSlotIndex;

    int32 LastLastSlotIndex;

    FTimerHandle ExchangeInventoryItensTimer;

    bool bExchangeInventoryItensEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Items|Inventory",
      meta = (AllowPrivateAccess = "true"))
    float ExchangeInventoryItensTime;

    bool bDebugSlotMessages;

    /** The index for the currently highlighted slot */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Items|Inventory",
      meta = (AllowPrivateAccess = "true"))
    int32 HighlightedSlot;

    float GlobalDeltaTime;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Custom Properties|Decals",
      meta = (AllowPrivateAccess = "true"))
    class UDecalComponent *BulletHoleDecal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Custom Properties|Decals",
      meta = (AllowPrivateAccess = "true"))
    class UMaterialInterface *BulletHoleDecalMat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag",
      meta     = (AllowPrivateAccess = "true"))
    bool bDisableCameraLagWhenMoving;

    EMovingDirection MovingDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = Category = "My Custom Properties|My Custom Camera|Custom Camera Lag",
      meta                = (AllowPrivateAccess = "true"))
    bool ShowEMovingDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Idle",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetIdle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Idle",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Idle;

    FVector OriginalCameraSocketOffset;

    float OriginalCameraLagSpeed;
    float OriginalCameraLagMaxDistance;

    FVector CustomCameraSocketOffset;
    float   CustomCameraLagSpeed;
    float   CustomCameraLagMaxDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetFroward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Froward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float Froward_InterpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Backwards;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float Backwards_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Right;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float Right_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Left;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float Left_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardRight_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardLeft_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardRight_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Running",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardLeft_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Aim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float Aim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetRightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_RightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float RightAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetLeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_LeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float LeftAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardRightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardRightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardRightAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardLeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardLeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardLeftAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardRightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardRightAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardRightAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardLeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardLeftAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Aim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardLeftAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Jump",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetJump;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Jump",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_Jump;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Jump",
      meta     = (AllowPrivateAccess = "true"))
    float Jump_interpTime;

    bool bIsJumping;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetRightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_RightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float RightCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetLeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_LeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float LeftCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardRightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardRightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardRightCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardLeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardLeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardLeftCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardRightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardRightCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardRightCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardLeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardLeftCrouch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|Crouch",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardLeftCrouch_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_CrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float CrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetRightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_RightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float RightCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetLeftCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_LeftCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float LeftCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardRightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardRightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardRightCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetForwardLeftCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_ForwardLeftCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float ForwardLeftCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardRightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardRightCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardRightCrouchAim_interpTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    FVector OffsetBackwardLeftCrouchAim;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    double CameraLagMaxDistance_BackwardLeftCrouchAim;
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|My Custom Camera|Custom Camera Lag|CrouchAim",
      meta     = (AllowPrivateAccess = "true"))
    float BackwardLeftCrouchAim_interpTime;

    bool bNotMoving;

public:

    /**
     * @brief Returns CameraBoom subObject.
     */
    FORCEINLINE USpringArmComponent *GetCameraBoom() const { return CameraBoom; };

    /** Returns FollowCamera subObject */
    FORCEINLINE UCameraComponent *GetFollowCamera() const { return FollowCamera; }

    FORCEINLINE bool GetAiming() const { return bAiming; }

    UFUNCTION(BlueprintCallable)
    float GetCrosshairHeight() const;

    UFUNCTION(BlueprintCallable)
    float GetCrosshairSpreadMultiplier() const;

    FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

    /**
     * @brief Adds/Subtract to/from OverlappedItemCount and updates
     *
     * bShouldTraceForItems
     * @param Amount The amount to
     * change OverlappedItemCount
     */
    void IncrementOverlappedItemCount(int8 Amount);

    FVector GetCameraInterpLocation();

    void GetPickupItem(AItem *Item);

    FORCEINLINE ECombatState GetCombatState() const { return CombatState; }

    FORCEINLINE bool GetCrouching() const { return bCrouching; }

    FInterpLocation GetInterpLocation(int32 Index);

    /**
     * @return the index in InterpLocations array
     * with the
     * lowest ItemCount
     */
    int32 GetInterpLocationIndex();

    void IncrementInterpLocItemCount(int32 Index, int32 Amount);

    FORCEINLINE bool ShouldPlayPickupSound() const { return bShouldPlayPickupSound; }
    FORCEINLINE bool ShouldPlayEquipSound() const { return bShouldPlayEquipSound; }

    void StartPickupSoundTimer();
    void StartEquipSoundTimer();
    void UnHighlightInventorySlot();

    FORCEINLINE AWeapon *GetEquippedWeapon() const { return EquippedWeapon; }
};
