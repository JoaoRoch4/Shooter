#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Custom.h"

#include "Item.generated.h"

using namespace UnrealBasic;

UENUM(BlueprintType, Category = "UEnus|EItemRarity")
enum class EItemRarity : uint8 {

    EIR_Damege    UMETA(DisplayName = "Damaged"),

    EIR_Common    UMETA(DisplayName = "Common"),

    EIR_Uncommon  UMETA(DisplayName = "Uncommon"),

    EIR_Rare      UMETA(DisplayName = "Rare"),

    EIR_Legendary UMETA(DisplayName = "Legendary"),

    EIR_MAX       UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType, Category = "UEnus|EItemState")
enum class EItemState : uint8 {

    EIS_Pickup         UMETA(DisplayName = "Pickup"),

    EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),

    EIS_PickedUp       UMETA(DisplayName = "PickedUp"),

    EIS_Equipped       UMETA(DisplayName = "Equipped"),

    EIS_Falling        UMETA(DisplayName = "Falling"),

    EIS_MAX            UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType, Category = "UEnus|EItemType")
enum class EItemType : uint8 {

    EIT_Ammo   UMETA(DisplayName = "Ammo"),

    EIT_Weapon UMETA(DisplayName = "Weapon"),

    EIT_MAX    UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SHOOTER_API AItem : public AActor {

    GENERATED_BODY()

public:

  // Sets default values for this actor's properties
    AItem();

protected:

  // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void OnConstruction(const FTransform &Transform) override;

    void DefaultConstructor_Curves();
     
    /**@brief Called when item is overlapped with AreaSphere. */
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    /** Called when end overlapping with AreaSphere */
    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

    /** *@brief Sets active stars array based on rarity. */
    void SetActiveStars();

    /**
     * @brief Sets Properties of itens components based on state.
     * @param State
     */
    virtual void SetItemProperties(EItemState State);

    /** *@brief Called when ItemInterpTimer is finished. */
    void FinishInterping();

    /**
     * @brief Handles item interpolation when in the
     * Equip interping state.
     * @param DeltaTime
     */
    void ItemInterp(float DeltaTime);

    /**
    * @brief
    * @return Interp location based on the item type
    */
    FVector GetInterpLocation();

    void PlayPickupSound();

    virtual void InitializeCustomDepth();

    UFUNCTION(BlueprintCallable)
    void StartPulseTimer();

    UFUNCTION(BlueprintCallable)
    void ResetPulseTimer();

    UFUNCTION(BlueprintCallable)
    void UpdatePulse();

public:

    virtual void Tick(float DeltaTime) override;

private:

    /**@brief Default constructor for Collision Box. */
    void DefaultConstructor_CollisionBox();

    /**@brief Skeletal mesh for the item. */
    UPROPERTY(VisibleAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Mesh",
        meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *ItemMesh;

    /**@brief Line trace collides with box to show HUD widgets. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Mesh",
        meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *CollisionBox;

    /**@brief Popup widget for when the player look at item. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent *PickupWidget;

    /**@brief Enables item tracing when overlapped. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    class USphereComponent *AreaSphere;

    /**@brief The name which appears on the pickup widget. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    FString ItemName;

    /**@brief The item count for this pickup. */
    UPROPERTY(EditAnywhere,
        BlueprintReadWrite,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    int32 ItemCount;

    /**@brief The item rarity determines the number of stars in the pickup widget. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget|Rarity",
        meta = (AllowPrivateAccess = "true"))
    EItemRarity ItemRarity;

    /** *@brief State of the item. */
    UPROPERTY(VisibleAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    EItemState ItemState;

    /**@brief Array of bools for each star in pickup widget. */
    UPROPERTY(VisibleAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interface|PickupWidget|Rarity",
        meta = (AllowPrivateAccess = "true"))
    TArray<bool> ActiveStars;

    /**
    *@brief The curve asset to use for the item's Z location
    * when interping.
    */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interp|Curves",
        meta = (AllowPrivateAccess = "true"))
    class UCurveFloat *ItemZ_Curve;

    /** *@brief Starting location when interping begins. */
    UPROPERTY(BlueprintReadOnly,
        Category = "My Custom Properties|Interp",
        meta = (AllowPrivateAccess = "true"))
    FVector ItemInterpStartLocation;

    /** *@brief Target interp location in front of the camera. */
    UPROPERTY(BlueprintReadOnly,
        Category = "My Custom Properties|Interp",
        meta = (AllowPrivateAccess = "true"))
    FVector CameraTargetLocation;

    /** *@brief True when interping. */
    UPROPERTY(BlueprintReadOnly,
        Category = "My Custom Properties|Interp",
        meta = (AllowPrivateAccess = "true"))
    bool bInterping;

    /** *@brief Plays when we start interping. */
    FTimerHandle ItemInterpTimer;

    /** *@brief Duration of the curve and timer. */
    UPROPERTY(EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "My Custom Properties|Interp",
        meta = (AllowPrivateAccess = "true"))
    float Z_CurveTime;

    /** *@brief Pointer to the character. */
    UPROPERTY(BlueprintReadOnly,
        Category = "My Custom Properties|Interp|Character",
        meta = (AllowPrivateAccess = "true"))
    class AShooterCharacter *Character;

    /** *@brief X for the Item while interping in the EquipInterping State. */
    float ItemInterpX;

    /** *@brief Y for the Item while interping in the EquipInterping State. */
    float ItemInterpY;

    /** *@brief Initial Yaw offset between the camera and the interping item. */
    float InterpInitialYawOffset;

    /** *@brief Curve used for scale the item when interping. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "My Custom Properties|Interp|Curves",
        meta = (AllowPrivateAccess = "true"))
    UCurveFloat *ItemScaleCurve;

    /** *@brief Sound played when item is picked up. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Interp|Sound",
        meta = (AllowPrivateAccess = "true"))
    class USoundCue *PickupSound;

    /** *@brief Sound played when item is equipped. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Interp|Sound",
        meta = (AllowPrivateAccess = "true"))
    USoundCue *EquipSound;

    /** *@brief Enum for the type this item is. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Interface|PickupWidget",
        meta = (AllowPrivateAccess = "true"))
    EItemType ItemType;

    /**
      *@brief Index for the interp location this item is
      *	interping to.
      */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "My Custom Properties|Interp",
        meta = (AllowPrivateAccess = "true"))
    int32 InterpLocIndex;

    /** *@brief Index for the material we'd like to change at runtime. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Materials",
        meta = (AllowPrivateAccess = "true"))
    int32 MaterialIndex;

    /** *@brief Dynamic instance that we can change at runtime. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "My Custom Properties|Materials",
        meta = (AllowPrivateAccess = "true"))
    UMaterialInstanceDynamic *DynamicMaterialInstance;

    /** *@brief Material instance used with the Dynamic Material instance. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Materials",
        meta = (AllowPrivateAccess = "true"))
    UMaterialInstance *MaterialInstance;

    bool bCanChangeCustomDepth;

    /** Curve to drive the dynamic material parameters */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Interp|Curves|PulseCurve", 
        meta = (AllowPrivateAccess = "true"))
    class UCurveVector *PulseCurve;

    /** Curve to drive the dynamic material parameters */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "My Custom Properties|Interp|Curves|PulseCurve",
        meta = (AllowPrivateAccess = "true"))
    UCurveVector *InterpPulseCurve;

    UPROPERTY(BlueprintReadOnly,
        Category = "My Custom Properties|Interp|Curves|PulseCurve",
        meta = (AllowPrivateAccess = "true"))
    FTimerHandle PulseTimer;

    UPROPERTY(VisibleAnywhere,
        Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
        meta = (AllowPrivateAccess = "true"))
    float GlowAmount;

    UPROPERTY(VisibleAnywhere,
        Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
        meta = (AllowPrivateAccess = "true"))
    float FresnelExponent;

    UPROPERTY(VisibleAnywhere, 
        Category = "My Custom Properties|Interp|Curves|PulseCurve|Values", 
        meta = (AllowPrivateAccess = "true"))
    float FresnelReflectFraction;

    /** Time for the PulseTimer */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
        meta = (AllowPrivateAccess = "true"))
    float PulseCurveTime;

    /** Background for this item in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "My Custom Properties|Interface|Inventory",
        meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconBackground;

	/** Icon for this item in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "My Custom Properties|Interface|Inventory",
        meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconItem;

public:

    FORCEINLINE UWidgetComponent *GetPickupWidget() const {
        return PickupWidget;
    }

    FORCEINLINE USphereComponent *GetAreaSphere() const {
        return AreaSphere;
    }

    FORCEINLINE UBoxComponent *GetCollisionBox() const {
        return CollisionBox;
    }

    FORCEINLINE EItemState GetItemState() const {
        return ItemState;
    }

    FORCEINLINE int32 GetItemCount() const {
        return ItemCount;
    }

    FORCEINLINE void SetItemCount(int32 Count) {
        ItemCount = Count;
    }

    void SetItemState(EItemState State);

    FORCEINLINE USkeletalMeshComponent *GetItemMesh() const {
        return ItemMesh;
    }

    FORCEINLINE USoundCue *GetPickupSound() const {
        return PickupSound;
    }

    FORCEINLINE USoundCue *GetEquipSound() const {
        return EquipSound;
    }

  /**
   * @brief Called from the AShooterCharacter class.
   * @param Char The character that is picking up the item.
   */
    void StartItemCurve(AShooterCharacter *Char);

    void PlayEquipSound();

    virtual void EnableCustomDepth();

    virtual void DisableCustomDepth();

    void EnableGlowMaterial();

    void DisableGlowMaterial();

};
