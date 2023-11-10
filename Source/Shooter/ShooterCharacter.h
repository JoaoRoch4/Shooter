#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AmmoType.h"
#include "Custom.h"

#include "ShooterCharacter.generated.h"

using namespace UnrealAll;

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

    /** *@brief Scene component to use for its location for interping. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent *SceneComponent {};

    /** *@brief Number of items interping to / at this scene com location.. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ItemCount {};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
  FEquipItemDelegate, int32, CurrentSlotIndex, int32, NewSlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHighlightIconDelegate, int32, SlotIndex, bool, bStartAnimation);
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

    void CalculateCrosshairSpread(float DeltaTime);

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

    /**
	 * @brief Interps capsule half height when crouching/standing.

     * * @param DeltaTime Called every frame
	 */
    void InterpCapsuleHalfHeight(float DeltaTime);

    void Aim();
    void StopAiming();

    void PickupAmmo(class AAmmo *Ammo);

    void InitializeInterpLocations();

    FORCEINLINE void ResetPickupSoundTimer();
    FORCEINLINE void ResetEquipSoundTimer();

    void ExchangeInventoryItens(int32 CurrentItemindex, int32 NewItemIndex);

    UFUNCTION(BlueprintCallable)
    void EnableExchangeInventoryItens();

    void DebugSlotsItens();
    void UpdateSlotsItens();

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

    /* Key Methods */
    void KeyMethodFKey();
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


private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Mesh",
      meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent *PlayerMesh;

    class TUniquePtr<ConstructorHelpers::FObjectFinder<USkeletalMesh>> SkeletalMeshContainer;

    /**@brief Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Camera",
      meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CameraBoom;

    /**@brief Camera that follows the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Camera",
      meta = (AllowPrivateAccess = "true"))
    class UCameraComponent *FollowCamera;

    /**
	 * @brief Is transitioning for cinematic camera to normal
     * camera
	 *  or vice versa.
	 */
    UPROPERTY()
    bool bIsTransitioning;

    /** Is true for Cinematic camera */
    UPROPERTY()
    bool bCinematicCameraSwitch;

    /** Use bezier curve for Cinematic camera */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    bool bUseBezierCurve;

    /** Camera Arm Length Start Editable on editor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float CameraArmLengthStart;

    /** Camera Arm Length End Editable on editor */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float CameraArmLengthEnd;

    /** Duration of the transition */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float TransitionDuration;

    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float PreviousYaw;

    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float LerpedArmLength;

    /** Current time of the transition */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Camera|Cinematic Camera",
      meta = (AllowPrivateAccess = "true"))
    float CurrentTime;

    /** Base turn rate, in deg / sec. Other scaling may affect final turn rate
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final turn
     * rate */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float BaseLookUpRate;

    /** Turn rate while not aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float HipTurnRate;

    /** Look up rate while not aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float HipLookUpRate;

    /** Turn rate while aiming */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float AimingTurnRate;

    /** *@brief Look up rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess))
    float AimingLookUpRate;

    /** *@brief Mouse turn rate while not aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseHipTurnRate;

    /** *@brief Mouse look up rate while not aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseHipLookUpRate;

    /** *@brief Mouse turn rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseAimingTurnRate;

    /** *@brief Mouse look up rate while aiming. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float MouseAimingLookUpRate;

    /** *@brief true if is firing the weapon. */
    UPROPERTY(
      BlueprintReadOnly, Category = "My Custom Properties|Combat|Fire", meta = (AllowPrivateAccess = "true"))
    bool bIsFiringWeapon;

    /** *@brief true if is firing the weapon. */
    UPROPERTY(
      BlueprintReadOnly, Category = "My Custom Properties|Combat|Fire", meta = (AllowPrivateAccess = "true"))
    bool bDidFire;

    /** *@brief Randomize gun shots sound cue. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    class USoundCue *FireSound;

    /** *@brief Flash spawned at Barrel socket. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    class UParticleSystem *MuzzleFlash;

    /** *@brief Particles spawned upon bullet impact. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UParticleSystem *ImpactParticles;

    /** *@brief Smoke trail for bullets. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UParticleSystem *BeamParticles;

    /** *@brief Montage for firing the weapon. */
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
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess = "true"))
    bool bAiming;

    /** *@brief Default camera field of view value. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess = "true"))
    float CameraDefaultAimFOV;

    /** *@brief Field of view value for when zoomed in. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess = "true"))
    float CameraZoomedAimFOV;

    /** *@brief Current field of view this frame. */
    UPROPERTY(
      BlueprintReadOnly, Category = "My Custom Properties|Camera|Aim", meta = (AllowPrivateAccess = "true"))
    float CameraCurrentAimFOV;

    /** *@brief Interp speed for zooming when aiming. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|Aim",
      meta = (AllowPrivateAccess = "true"))
    float ZoomInterpSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairHeight;

    /** *@brief Determines the spread of the crosshairs. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairSpreadMultiplier;

    /** *@brief Velocity component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairVelocityFactor;

    /** *@brief In air component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairInAirFactor;

    /** *@brief Aim component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairAimFactor;

    /** *@brief Shooting component for crosshairs spread. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat|Crosshairs",
      meta = (AllowPrivateAccess = "true"))
    float CrosshairShootingFactor;

    float ShootTimeDuration;

    bool bFiringBullet;

    FTimerHandle CrosshairShootTimer;

    /** *@brief Left mouse button or right console trigger pressed. */
    bool bFireButtonPressed;

    /** *@brief True when we can fire. False when waiting for the timer. */
    bool bShouldFire;

    /** *@brief Rate of automatic gun fire. */
    float AutomaticFireRate;

    /** *@brief Sets a timer between gun shots. */
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
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
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
    UPROPERTY(
      BlueprintReadOnly, Category = "My Custom Properties|Combat|Class", meta = (AllowPrivateAccess = "true"))
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

    /** *@brief Map to keep track of ammo of different types. */
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

    /** *@brief Combat State. Cannot fire or reload when unoccupied. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Combat",
      meta = (AllowPrivateAccess = "true"))
    ECombatState CombatState;

    /** @brief Montage for reloading animations. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Class",
      meta = (AllowPrivateAccess = "true"))
    UAnimMontage *ReloadMontage;

    /** *@brief Transform of the clip when we first grab the clip during
     * reloading. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Animation",
      meta = (AllowPrivateAccess = "true"))
    FTransform ClipTransform;

    /** *@brief Scene component to attach to the Character's hand during
     * reloading. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Animation",
      meta = (AllowPrivateAccess = "true"))
    USceneComponent *HandSceneComponent;

    /** @brief True when crouching. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    bool bCrouching;

    /** *@brief Regular Movement speed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float BaseMovementSpeed;

    /** *@brief Crouch Movement Speed. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchMovementSpeed;

    /** *@brief Current Half Height of the Capsule. */
    float CurrentCapsuleHalfHeight;

    /** *@brief Half Height of the Capsule when not Crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float StandingCapsuleHalfHeight;

    /** *@brief Half Height of the Capsule when Crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchingCapsuleHalfHeight;

    /** *@brief Ground Friction while not crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float BaseGroundFriction;

    /** *@brief Ground Friction while crouching. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Combat|Movement",
      meta = (AllowPrivateAccess = "true"))
    float CrouchingGroundFriction;

    /** *@brief Used for knowing the Aiming Button is Pressed. */
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

    /** *@brief Array of interp location structs. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Items|Interpolation Scene Components",
      meta     = (AllowPrivateAccess = "true"))
    TArray<FInterpLocation> InterpLocations;

    FTimerHandle PickupSoundTimer;
    FTimerHandle EquipSoundTimer;

    bool bShouldPlayPickupSound;
    bool bShouldPlayEquipSound;

    /** *@brief Time to wait before we can play another Pickup Sound. */
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

public:
    /**
	 * @brief Returns Cameraboom subObject.
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
};
