#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {

	EWT_SubmachineGun  UMETA(DisplayName = "Sub-machine Gun"),
	EWT_AssaultRifle   UMETA(DisplayName = "Assault Rifle"),

	EWT_MAX            UMETA(DisplayName = "DefaultMAX")
};

/**
 *
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	void SyncItemMunition();

	void StopFalling();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon",
		meta = (AllowPrivateAccess = "true"))
	float ThrowWeaponTime;

	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon",
		meta = (AllowPrivateAccess = "true"))
	bool bNotRandValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "bNotRandValues"))
	float ThrowHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "bNotRandValues"))
	FVector ThrowDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Static",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "bNotRandValues"))
	float MultiplyImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "!bNotRandValues"))
	FVector2D ThrowHeightRandRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "!bNotRandValues"))
	FVector2D AddAdiotionalRotationRandRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item|Throw Weapon|Impulse|Rand Range",
		meta = (AllowPrivateAccess = "true"),
		meta = (EditCondition = "!bNotRandValues"))
	FVector2D MultiplyImpulseRandRange;

	UPROPERTY()
	FTimerHandle ThrowWeaponTimer;

	/** *@brief Ammo count for this weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	/** *@brief The maximum ammo count for this weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	int32 MagazineCapacity;

	/** *@brief The type of weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	/** *@brief The type of ammo for this weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	/** *@brief FName for the reload montage section. */
	UPROPERTY(BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;

	/** *@brief True when moving the clip while reloading. */
	UPROPERTY(BlueprintReadOnly,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	bool bMovingClip;

	/** *@brief Name for the clip bone. */
	UPROPERTY(BlueprintReadWrite,
		Category = "My Custom Properties|Item",
		meta = (AllowPrivateAccess = "true"))
	FName ClipBoneName;

	/** *@brief Pointer to the item class. */
	class AItem *ItemInstance;

public:

	/** Adds an impulse to the weapon */
	UFUNCTION(BlueprintCallable)
	void ThrowWeapon();

	FORCEINLINE int32 GetAmmo() const {
		return Ammo;
	}

	FORCEINLINE int32 GetMagazineCapacity() const {
		return MagazineCapacity;
	}

	/**
	 * @brief Called From Character Class when firing
	 * Weapon.
	 */
	void DecrementAmmo();

	FORCEINLINE EWeaponType GetWeaponType() const {
		return WeaponType;
	}

	FORCEINLINE EAmmoType GetAmmoType() const {
		return AmmoType;
	}

	FORCEINLINE FName GetReloadMontageSection() const {
		return ReloadMontageSection;
	}

	FORCEINLINE FName GetClipBoneName() const {
		return ClipBoneName;
	}

	void ReloadAmmo(int32 Amount);

	FORCEINLINE void SetMovingClip(bool Move) {
		bMovingClip = Move;
	}

	FORCEINLINE bool ClipIsFull() const;
};
