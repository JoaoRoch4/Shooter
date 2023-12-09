#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"

#include "Custom.h"

#include "Item.generated.h"

using namespace UnrealBasic;

UENUM(BlueprintType, Category = "UEnums|EItemRarity")
enum class EItemRarity : uint8 {

    EIR_Damaged UMETA(DisplayName = "Damaged"),

    EIR_Common UMETA(DisplayName = "Common"),

    EIR_Uncommon UMETA(DisplayName = "Uncommon"),

    EIR_Rare UMETA(DisplayName = "Rare"),

    EIR_Legendary UMETA(DisplayName = "Legendary"),

    EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType, Category = "UEnums|EItemState")
enum class EItemState : uint8 {

    EIS_Pickup UMETA(DisplayName = "Pickup"),

    EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),

    EIS_PickedUp UMETA(DisplayName = "PickedUp"),

    EIS_Equipped UMETA(DisplayName = "Equipped"),

    EIS_Falling UMETA(DisplayName = "Falling"),

    EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType, Category = "UEnums|EItemType")
enum class EItemType : uint8 {

    EIT_Ammo UMETA(DisplayName = "Ammo"),

    EIT_Weapon UMETA(DisplayName = "Weapon"),

    EIT_MAX UMETA(DisplayName = "DefaultMAX")
};

USTRUCT(BlueprintType, Category = "DataTables|ItemRarityTable")
struct FItemRarityTable : public FTableRowBase {

    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor GlowColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor LightColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor DarkColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NumberOfStars;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *IconBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CustomDepthStencil;

    FItemRarityTable()
     : GlowColor(FLinearColor())
     , LightColor(FLinearColor())
     , DarkColor(FLinearColor())
     , NumberOfStars(0)
     , IconBackground(nullptr)
     , CustomDepthStencil(0) {}
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

    /* Called when item is overlapped with AreaSphere. */
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
      const FHitResult &SweepResult);

    /** Called when end overlapping with AreaSphere */
    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

    /**  Sets active stars array based on rarity. */
    void SetActiveStars();

    /**
	 * @brief Sets Properties of itens components based on state.

     * * @param State
	 */
    virtual void SetItemProperties(EItemState State);

    /**  Called when ItemInterpTimer is finished. */
    void FinishInterping();

    /**
	 * @brief Handles item interpolation when in the
	 * Equip
     * interping state.
	 * @param DeltaTime
	 */
    void ItemInterp(float DeltaTime);

    /**
	* @brief
	* @return Interp location based on the item
     * type
	*/
    FVector GetInterpLocation();

    void PlayPickupSound(bool bForcePlaySound = false);

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
    /* Default constructor for Collision Box. */
    void DefaultConstructor_CollisionBox();

    /* Skeletal mesh for the item. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Mesh",
      meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *ItemMesh;

    /* Line trace collides with box to show HUD widgets. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Mesh",
      meta = (AllowPrivateAccess = "true"))
    class UBoxComponent *CollisionBox;

    /* Popup widget for when the player look at item. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    class UWidgetComponent *PickupWidget;

    /* Enables item tracing when overlapped. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    class USphereComponent *AreaSphere;

    /* The name which appears on the pickup widget. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    FString ItemName;

    /* The item count for this pickup. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    int32 ItemCount;

    /* The item rarity determines the number of stars in the pickup
     * widget. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity",
      meta     = (AllowPrivateAccess = "true"))
    EItemRarity ItemRarity;

    /**  State of the item. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    EItemState ItemState;

    /* Array of bools for each star in pickup widget. */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity",
      meta     = (AllowPrivateAccess = "true"))
    TArray<bool> ActiveStars;

    /**	The curve asset to use for the item's Z location when interping. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Interp|Curves",
      meta = (AllowPrivateAccess = "true"))
    class UCurveFloat *ItemZ_Curve;

    /**  Starting location when interping begins. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Interp",
      meta = (AllowPrivateAccess = "true"))
    FVector ItemInterpStartLocation;

    /**  Target interp location in front of the camera. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Interp",
      meta = (AllowPrivateAccess = "true"))
    FVector CameraTargetLocation;

    /**  True when interping. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Interp",
      meta = (AllowPrivateAccess = "true"))
    bool bInterping;

    /**  Plays when we start interping. */
    FTimerHandle ItemInterpTimer;

    /**  Duration of the curve and timer. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Interp",
      meta = (AllowPrivateAccess = "true"))
    float Z_CurveTime;

    /**  Pointer to the character. */
    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Interp|Character",
      meta = (AllowPrivateAccess = "true"))
    class AShooterCharacter *Character;

    /**  X for the Item while interping in the EquipInterping State. */
    float ItemInterpX;

    /**  Y for the Item while interping in the EquipInterping State. */
    float ItemInterpY;

    /**  Initial Yaw offset between the camera and the interping item. */
    float InterpInitialYawOffset;

    /**  Curve used for scale the item when interping. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties|Interp|Curves",
      meta = (AllowPrivateAccess = "true"))
    UCurveFloat *ItemScaleCurve;

    /**  Sound played when item is picked up. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Interp|Sound",
      meta = (AllowPrivateAccess = "true"))
    class USoundCue *PickupSound;

    /**  Sound played when item is equipped. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Interp|Sound",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *EquipSound;

    /**  Enum for the type this item is. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Interface|PickupWidget",
      meta     = (AllowPrivateAccess = "true"))
    EItemType ItemType;

    /**
	   Index for the interp location this item is
	  *
     * interping to.
	  */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Interp",
      meta = (AllowPrivateAccess = "true"))
    int32 InterpLocIndex;

    /**  Index for the material we'd like to change at runtime. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Materials",
      meta = (AllowPrivateAccess = "true"))
    int32 MaterialIndex;

    /**  Dynamic instance that we can change at runtime. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Materials",
      meta = (AllowPrivateAccess = "true"))
    UMaterialInstanceDynamic *DynamicMaterialInstance;

    /**  Material instance used with the Dynamic Material instance. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Materials",
      meta = (AllowPrivateAccess = "true"))
    UMaterialInstance *MaterialInstance;

    bool bCanChangeCustomDepth;

    /** Curve to drive the dynamic material parameters */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Interp|Curves|PulseCurve",
      meta     = (AllowPrivateAccess = "true"))
    class UCurveVector *PulseCurve;

    /** Curve to drive the dynamic material parameters */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
      Category = "My Custom Properties|Interp|Curves|PulseCurve",
      meta     = (AllowPrivateAccess = "true"))
    UCurveVector *InterpPulseCurve;

    UPROPERTY(BlueprintReadOnly, Category = "My Custom Properties|Interp|Curves|PulseCurve",
      meta = (AllowPrivateAccess = "true"))
    FTimerHandle PulseTimer;

    UPROPERTY(VisibleAnywhere, Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
      meta = (AllowPrivateAccess = "true"))
    float GlowAmount;

    UPROPERTY(VisibleAnywhere, Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
      meta = (AllowPrivateAccess = "true"))
    float FresnelExponent;

    UPROPERTY(VisibleAnywhere, Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
      meta = (AllowPrivateAccess = "true"))
    float FresnelReflectFraction;

    /** Time for the PulseTimer */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Interp|Curves|PulseCurve|Values",
      meta     = (AllowPrivateAccess = "true"))
    float PulseCurveTime;

    /** Icon for this item in the inventory */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|",
      meta     = (AllowPrivateAccess = "true"))
    UTexture2D *IconItem;

    /** Ammo Icon for this item in the inventory */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|",
      meta     = (AllowPrivateAccess = "true"))
    UTexture2D *AmmoItem;

    /** Slot in the inventory array */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties",
      meta = (AllowPrivateAccess = "true"))
    int32 SlotIndex;

    /** True when the Character's inventory is full */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "My Custom Properties",
      meta = (AllowPrivateAccess = "true"))
    bool bCharacterInventoryFull;

    /** item Rarity Data table */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    class UDataTable *ItemRarityDataTable;

    /** Color in the glow material */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    FLinearColor GlowColor;

    /** Light color in the pickup widget */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    FLinearColor LightColor;

    /** Dark color in the pickup widget */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    FLinearColor DarkColor;

    /** Number of Stars in the pickup widget */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    int32 NumberOfStars;

    /** Background icon for the inventory */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
      Category = "My Custom Properties|Interface|PickupWidget|Rarity|DataTable",
      meta     = (AllowPrivateAccess = "true"))
    UTexture2D *IconBackground;

public:
    FORCEINLINE UWidgetComponent *GetPickupWidget() const { return PickupWidget; }

    FORCEINLINE USphereComponent *GetAreaSphere() const { return AreaSphere; }

    FORCEINLINE UBoxComponent *GetCollisionBox() const { return CollisionBox; }

    FORCEINLINE EItemState GetItemState() const { return ItemState; }

    FORCEINLINE int32 GetItemCount() const { return ItemCount; }

    FORCEINLINE void SetItemCount(int32 Count) { ItemCount = Count; }

    void SetItemState(EItemState State);

    FORCEINLINE USkeletalMeshComponent *GetItemMesh() const { return ItemMesh; }

    FORCEINLINE USoundCue *GetPickupSound() const { return PickupSound; }

    FORCEINLINE USoundCue *GetEquipSound() const { return EquipSound; }

    /**
     * @brief Called from the AShooterCharacter class.
     * @param Char
     * The character that is picking up the item.
     */
    void StartItemCurve(AShooterCharacter *GetCharacter, bool bForcePlaySound = false);

    void PlayEquipSound(bool bForcePlaySound = false);

    virtual void EnableCustomDepth();

    virtual void DisableCustomDepth();

    void EnableGlowMaterial();

    void DisableGlowMaterial();

    FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }
    FORCEINLINE void  SetSlotIndex(int32 Index) { SlotIndex = Index; }
    FORCEINLINE void  SetCharacter(AShooterCharacter *GetCharacter) { Character = GetCharacter; }
    FORCEINLINE void  SetCharacterInventoryFull(bool bIsFull) { bCharacterInventoryFull = bIsFull; }
};
