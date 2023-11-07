#pragma once

#include "CoreMinimal.h"

#include "Item.h"

#include "AmmoType.h"
#include "Ammo.generated.h"

using namespace UnrealBasic;

/**
 * 
 */
UCLASS()
class SHOOTER_API AAmmo : public AItem {
    GENERATED_BODY()

public:
    AAmmo();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

    /**
	 * @brief Override of SetItemProperties so we can set AmmoMesh

     * * Properties.
	 * @param State The item state to set the item. Using
     * the EItemState
	 * enum.
	 */
    virtual void SetItemProperties(EItemState State) override;

    UFUNCTION()
    void AmmoSphereOverlap(UPrimitiveComponent *OverlappedComponent,
        AActor *OtherActor,
        UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult);


private:
    /** *@brief Mesh for the Ammo Pickup. */
    UPROPERTY(VisibleAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Item",
        meta     = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *AmmoMesh;

    /** *@brief The type of ammo. */
    UPROPERTY(EditAnywhere,
        BlueprintReadWrite,
        Category = "My Custom Properties|Item",
        meta     = (AllowPrivateAccess = "true"))
    EAmmoType AmmoType;

    /** *@brief The texture for the ammo icon. */
    UPROPERTY(EditAnywhere,
        BlueprintReadWrite,
        Category = "My Custom Properties|Mesh",
        meta     = (AllowPrivateAccess = "true"))
    UTexture2D *AmmoIconTexture;

    /** *@brief Overlap Sphere for picking up ammo. */
    UPROPERTY(EditAnywhere,
        BlueprintReadOnly,
        Category = "My Custom Properties|Mesh",
        meta     = (AllowPrivateAccess = "true"))
    class USphereComponent *AmmoCollisionSphere;

    

public:
    FORCEINLINE UStaticMeshComponent *GetAmmoMesh() const { return AmmoMesh; }

    FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

    virtual void EnableCustomDepth() override;

    virtual void DisableCustomDepth() override;

   
};
