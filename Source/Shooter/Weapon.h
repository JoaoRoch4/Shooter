#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "AmmoType.h"
#include "Item.h"
#include "WeaponDataTable.h"
#include "WeaponType.h"

#include "Weapon.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem {
    GENERATED_BODY()

public:

    AWeapon();

    virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;

    virtual void OnConstruction(const FTransform &Transform) override;

    void Construct_WeaponTableObject();

    void SetWeaponTableObject(UDataTable *WeaponTableObject);

    void SyncItemMunition();

    void StopFalling();

    void FinishMovingSlide();

    // updates the slide displacement on pistol
    void UpdateSlideDisplacement();

private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item|Throw Weapon",
      meta = (AllowPrivateAccess = "true"))
    float ThrowWeaponTime;

    bool bFalling;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item|Throw Weapon",
      meta = (AllowPrivateAccess = "true"))
    bool bNotRandValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "bNotRandValues"))
    float ThrowHeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "bNotRandValues"))
    FVector ThrowDirection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "bNotRandValues"))
    float MultiplyImpulse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D ThrowHeightRandRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D ThrowDirectionRandRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range|ThrowDirection",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D ThrowDirection_X_RandRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range|ThrowDirection",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D ThrowDirection_Y_RandRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range|ThrowDirection",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D ThrowDirection_Z_RandRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
      meta = (AllowPrivateAccess = "true"), meta = (EditCondition = "!bNotRandValues"))
    FVector2D MultiplyImpulseRandRange;

    UPROPERTY()
    FTimerHandle ThrowWeaponTimer;

    /**  Ammo count for this weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    int32 Ammo;

    /**  The maximum ammo count for this weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    int32 MagazineCapacity;

    /**  The type of weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    EWeaponType WeaponType;

    /**  The type of ammo for this weapon. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    EAmmoType AmmoType;

    /**  FName for the reload montage section. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    FName ReloadMontageSection;

    /**  True when moving the clip while reloading. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    bool bMovingClip;

    /**  Name for the clip bone. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    FName ClipBoneName;

    /**  Pointer to the item class. */
    class AItem *ItemInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Item",
      meta = (AllowPrivateAccess = "true"))
    int32 MaxSlotNumber {5};

    /**  The data table for weapon properties. */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|DataTable",
      meta = (AllowPrivateAccess = "true"))
    UDataTable *WeaponDataTable;

    int32 PreviousMaterialIndex;

    /** Textures for the weapon Crosshairs */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UTexture2D *CrosshairsMiddle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UTexture2D *CrosshairsLeft;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UTexture2D *CrosshairsRight;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UTexture2D *CrosshairsBotton;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UTexture2D *CrosshairsTop;

    /** The speed at which automatic fire happens */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    float AutoFireRate;

    /** Particle system spawned at the BarrelSocket */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    UParticleSystem *MuzzleFlash;

    /** Sound played when the weapon is fired */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    USoundCue *FireSound;

    /** Name of the bone to hide on the weapon mesh */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|DataTable|Crosshairs", meta = (AllowPrivateAccess = "true"))
    FName BoneToHide;

    /** Amount that the slide is pushed back during pistol fire */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    float SlideDisplacement;

    /** Curve for the slide displacement */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    class UCurveFloat *SlideDisplacementCurve;

    /** Timer Handle for updating SlideDisplacement */
    FTimerHandle SlideTimer;

    /** Time for displacing the slide during pistol fire */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    float SlideDisplacementTime;

    /** True when the slide is moving */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    bool bMovingSlide;

    /** Max distance for the slide on the pistol */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    float MaxSlideDisplacement;

    /** Max rotation for the recoil on the pistol */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    float MaxRecoilRotation;

    /** Amount that the pistol rotate during pistol fire */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    float RecoilRotation;

    /** True for auto gunfire */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Pistol",
      meta = (AllowPrivateAccess = "true"))
    bool bAutomatic;

public:

    /** Adds an im*pulse to the weapon */
    UFUNCTION(BlueprintCallable)
    void ThrowWeapon();

    FORCEINLINE int32 GetAmmo() const { return Ammo; }

    FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

    void DecrementAmmo();

    FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

    FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

    FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
    FORCEINLINE void  SetReloadMontageSection(const FName Section) {
        ReloadMontageSection = Section;
    }

    FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
    FORCEINLINE void  SetClipBoneName(const FName BoneName) { ClipBoneName = BoneName; }

    void ReloadAmmo(int32 Amount);

    FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }

    bool ClipIsFull() const;

    FORCEINLINE int32 GetMaxSlotNumber() const { return MaxSlotNumber; }

    FORCEINLINE float GetAutoFireRate() const { return AutoFireRate; }

    FORCEINLINE UParticleSystem *GetMuzzleFlash() const { return MuzzleFlash; }

    FORCEINLINE USoundCue *GetFireSound() const { return FireSound; }

    void StartSlideTimer();

    FORCEINLINE bool GetAutomatic() const { return bAutomatic; }
};
