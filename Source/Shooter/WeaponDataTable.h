#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataTable.generated.h"

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase {

    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAmmoType AmmoType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 WeaponAmmo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MagazineCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USoundCue *PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundCue *EquipSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USkeletalMesh *ItemMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTexture2D *InventoryIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *AmmoIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UMaterialInstance *MaterialInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaterialIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ClipBoneName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ReloadMontageSection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UAnimInstance> AnimBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *CrosshairsMiddle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *CrosshairsLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *CrosshairsRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *CrosshairsBotton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D *CrosshairsTop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AutoFireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UParticleSystem *MuzzleFlash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundCue *FireSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BoneToHide;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAutomatic;

    FWeaponDataTable()
     : AmmoType(EAmmoType::EAT_9mm)
     , WeaponAmmo(0)
     , MagazineCapacity(0)
     , PickupSound(nullptr)
     , EquipSound(nullptr)
     , ItemMesh(nullptr)
     , ItemName(FString())
     , InventoryIcon(nullptr)
     , AmmoIcon(nullptr)
     , MaterialInstance(nullptr)
     , MaterialIndex(0)
     , ClipBoneName(FName(TEXT("")))
     , ReloadMontageSection(FName(TEXT("")))
     , AnimBP(nullptr)
     , CrosshairsMiddle(nullptr)
     , CrosshairsLeft(nullptr)
     , CrosshairsRight(nullptr)
     , CrosshairsBotton(nullptr)
     , CrosshairsTop(nullptr)
     , AutoFireRate(0.f)
     , MuzzleFlash(nullptr)
     , FireSound(nullptr)
     , BoneToHide(FName(TEXT("")))
     , bAutomatic(true)    {};
};