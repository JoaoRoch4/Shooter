#include "Weapon.h"

#include "Custom.h"
#include "Item.h"

#include <Engine\Engine.h>
#include <Engine\World.h>
#include <Kismet\GameplayStatics.h>

AWeapon::AWeapon()
 : ThrowWeaponTime(0.7f)
 , bFalling(false)
 , bNotRandValues(false)
 , ThrowHeight(10.f)
 , ThrowDirection(FVector(0.f, 0.f, 0.f))
 , MultiplyImpulse(180.f)
 , ThrowHeightRandRange(FVector2D(90.f, 180.f))
 , ThrowDirectionRandRange(FVector2D(90.f, 180.f))
 , ThrowDirection_X_RandRange(FVector2D(0.f, 0.f))
 , ThrowDirection_Y_RandRange(FVector2D(0.f, 0.f))
 , ThrowDirection_Z_RandRange(FVector2D(0.f, 0.f))
 , MultiplyImpulseRandRange(FVector2D(150.f, 180.f))
 , ThrowWeaponTimer(FTimerHandle())
 , Ammo(40)
 , MagazineCapacity(120)
 , WeaponType(EWeaponType::EWT_SubmachineGun)
 , AmmoType(EAmmoType::EAT_9mm)
 , ReloadMontageSection(FName(L"Reload SMG"))
 , ClipBoneName(FName(L"smg_clip"))
 , ItemInstance(nullptr)
 , WeaponDataTable(nullptr)
 , PreviousMaterialIndex(NULL)
 , CrosshairsMiddle(nullptr)
 , CrosshairsLeft(nullptr)
 , CrosshairsRight(nullptr)
 , CrosshairsBotton(nullptr)
 , CrosshairsTop(nullptr)
 , AutoFireRate(NULL)
 , MuzzleFlash(nullptr)
 , FireSound(nullptr)
 , BoneToHide(FName(TEXT("")))
 , SlideDisplacement(NULL)
 , SlideDisplacementCurve(nullptr)
 , SlideTimer(FTimerHandle())
 , SlideDisplacementTime(0.2f)
 , bMovingSlide(false)
 , MaxSlideDisplacement(4.f)
 , MaxRecoilRotation(20.f)
 , RecoilRotation(NULL) {

    PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay() {

    Super::BeginPlay();

    if (BoneToHide != FName(TEXT(""))) {

        GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);
    }

    Construct_WeaponTableObject();

    // SyncItemMunition();
}

void AWeapon::OnConstruction(const FTransform &Transform) {

    Super::OnConstruction(Transform);

    Construct_WeaponTableObject();
}

void AWeapon::Construct_WeaponTableObject() {

    const FString WeaponTablePath {L"/Script/Engine.DataTable'"
                                   L"/Game/_Game/DataTable/WeaponDataTable.WeaponDataTable'"};

    UObject *GetWeaponTableObject {
      StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath)};

    UDataTable *WeaponTableObject {};

    if (GetWeaponTableObject) {

        WeaponTableObject = Cast<UDataTable>(GetWeaponTableObject);

    } else {

        ExitPrintErr(
          "AWeapon::OnConstruction()-> if (WeaponTableObject): GetWeaponTableObject was nullptr")
    }

    if (WeaponTableObject) {

        SetWeaponTableObject(WeaponTableObject);

    } else {

        ExitPrintErr(
          "AWeapon::OnConstruction()-> if (WeaponTableObject): WeaponTableObject was nullptr")
    }
}

void AWeapon::SetWeaponTableObject(UDataTable *WeaponTableObject) {

    CheckPtr(WeaponTableObject);
    FWeaponDataTable *WeaponDataRow {};

    switch (WeaponType) {

        case EWeaponType::EWT_SubmachineGun : {

            WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(
              FName(L"SubmachineGun"), TEXT(""), true);

        } break;

        case EWeaponType::EWT_AssaultRifle : {

            WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(
              FName(L"AssaultRifle"), TEXT(""), true);

        } break;

        case EWeaponType::EWT_Pistol: {

            WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(
              FName(L"Pistol"), TEXT(""), true);

        } break;

        case EWeaponType::EWT_MAX : {

            ExitPrintErr("AWeapon::SetWeaponTableObject(): -> switch (WeaponType):"
                         "EWeaponType::EWT_MAX cannot be used")
        };

        default : {

            ExitPrintErr("AWeapon::SetWeaponTableObject(): -> switch (WeaponType):"
                         "Invalid EWeaponType! default case reached!")
        };
    }

    CheckPtr(WeaponDataRow);
    CheckPtr(GetItemMesh());
    CheckPtr(GetMaterialInstance());
    CheckPtr(GetDynamicMaterialInstance());

    AmmoType         = WeaponDataRow->AmmoType;
    Ammo             = WeaponDataRow->WeaponAmmo;
    MagazineCapacity = WeaponDataRow->MagazineCapacity;
    SetPickupSound(WeaponDataRow->PickupSound);
    SetEquipSound(WeaponDataRow->EquipSound);
    GetItemMesh()->SetSkeletalMesh(WeaponDataRow->ItemMesh);
    SetItemName(WeaponDataRow->ItemName);
    SetIconItem(WeaponDataRow->InventoryIcon);
    SetAmmoIcon(WeaponDataRow->AmmoIcon);
    SetMaterialInstance(WeaponDataRow->MaterialInstance);
    PreviousMaterialIndex = GetMaterialIndex();
    GetItemMesh()->SetMaterial(PreviousMaterialIndex, nullptr);
    SetMaterialIndex(WeaponDataRow->MaterialIndex);
    SetClipBoneName(WeaponDataRow->ClipBoneName);
    SetDynamicMaterialInstance(UMaterialInstanceDynamic::Create(GetMaterialInstance(), this));
    GetDynamicMaterialInstance()->SetVectorParameterValue(L"FresnelColor", GetGlowColor());
    GetItemMesh()->SetMaterial(GetMaterialIndex(), GetDynamicMaterialInstance());
    SetReloadMontageSection(WeaponDataRow->ReloadMontageSection);
    GetItemMesh()->SetAnimInstanceClass(WeaponDataRow->AnimBP);
    CrosshairsMiddle = WeaponDataRow->CrosshairsMiddle;
    CrosshairsLeft   = WeaponDataRow->CrosshairsLeft;
    CrosshairsRight  = WeaponDataRow->CrosshairsRight;
    CrosshairsBotton = WeaponDataRow->CrosshairsBotton;
    CrosshairsTop    = WeaponDataRow->CrosshairsTop;
    AutoFireRate     = WeaponDataRow->AutoFireRate;
    MuzzleFlash      = WeaponDataRow->MuzzleFlash;
    FireSound        = WeaponDataRow->FireSound;
    BoneToHide       = WeaponDataRow->BoneToHide;
    GetItemMesh()->HideBoneByName(BoneToHide, EPhysBodyOp::PBO_None);

    EnableGlowMaterial();
}

void AWeapon::SyncItemMunition() {

    int32 ItemCapacity {};

    // ItemInstance = Cast<AItem>(UGameplayStatics::GetActorOfClass(
    // GetWorld(), AWeapon::StaticClass()));

    if (ItemInstance) {

        ItemCapacity = ItemInstance->GetItemCount();

        if (Ammo != ItemCapacity) {

            Ammo = ItemCapacity;

            ItemInstance->SetItemCount(ItemCapacity);
        }

    } else {

        PrintLogErr("ItemInstance was nullptr");
    }
}

void AWeapon::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);

    // Keep the weapon upright
    if (GetItemState() == EItemState::EIS_Falling && bFalling) {

        const FRotator MeshRotation {0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};

        GetItemMesh()->SetWorldRotation(
          MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
    }

    if (bMovingSlide)
    UpdateSlideDisplacement();
}

void AWeapon::ThrowWeapon() {

    FRotator MeshRotation {0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};

    GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

    const FVector MeshForward {GetItemMesh()->GetForwardVector()};
    const FVector MeshRight {GetItemMesh()->GetRightVector()};

    // Direction in which we throw the Weapon

    FVector ImpulseDirection {};
    double  ThrowDirectionX {};
    double  ThrowDirectionY {};
    double  ThrowDirectionZ {};

    if (bNotRandValues) {

        ThrowDirectionX  = ThrowDirection.X;
        ThrowDirectionY  = ThrowDirection.Y;
        ThrowDirectionZ  = ThrowDirection.Z;
        ImpulseDirection = MeshForward.RotateAngleAxis(ThrowHeight * -1.f, MeshRight);

    } else {

        // RR = FMath::RandRange
        double RandThrowDirectionX {RR(ThrowDirection_X_RandRange.X, ThrowDirection_X_RandRange.Y)};

        ThrowDirectionX = RandThrowDirectionX;

        double RandThrowDirectionY {RR(ThrowDirection_Y_RandRange.X, ThrowDirection_Y_RandRange.Y)};

        ThrowDirectionY = RandThrowDirectionY;

        double RandThrowDirectionZ {RR(ThrowDirection_Z_RandRange.X, ThrowDirection_Z_RandRange.Y)};

        ThrowDirectionZ = RandThrowDirectionZ;

        double RandThrowHeight {RR(ThrowHeightRandRange.X, ThrowHeightRandRange.Y)};

        RandThrowHeight *= -1.f;

        ImpulseDirection = MeshForward.RotateAngleAxis(RandThrowHeight, MeshRight);
    }

    ImpulseDirection = ImpulseDirection.RotateAngleAxis(ThrowDirectionX, FVector(1.f, 0.f, 0.f));

    ImpulseDirection = ImpulseDirection.RotateAngleAxis(ThrowDirectionY, FVector(0.f, 1.f, 1.f));

    ImpulseDirection = ImpulseDirection.RotateAngleAxis(ThrowDirectionZ, FVector(0.f, 0.f, 1.f));

    if (bNotRandValues) {

        ImpulseDirection = (ImpulseDirection *= (MultiplyImpulse *= 10000.f));

    } else {

        MultiplyImpulseRandRange *= 10000.f;
        double RandMultiplyImpulse {RR(MultiplyImpulseRandRange.X, MultiplyImpulseRandRange.Y)};

        ImpulseDirection *= RandMultiplyImpulse;
    }

    GetItemMesh()->AddImpulse(ImpulseDirection);

    bFalling = true;

    GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);

    EnableGlowMaterial();
}

void AWeapon::StopFalling() {

    bFalling = false;

    SetItemState(EItemState::EIS_Pickup);

    StartPulseTimer();
}

void AWeapon::FinishMovingSlide() {

    bMovingSlide = false; }

void AWeapon::UpdateSlideDisplacement() {
        
    CheckPtr(SlideDisplacementCurve);

    const float ElapsedTime {GetWorldTimerManager().GetTimerElapsed(SlideTimer)};
    const float CurveValue  {SlideDisplacementCurve->GetFloatValue(ElapsedTime)};

    SlideDisplacement = CurveValue * MaxSlideDisplacement;

    RecoilRotation = CurveValue * MaxRecoilRotation;
}

void AWeapon::DecrementAmmo() {

    if (Ammo - 1 <= 0) Ammo = 0;
    else --Ammo;
}

void AWeapon::ReloadAmmo(int32 Amount) {

    checkf(
      Ammo + Amount <= MagazineCapacity, L"Attempted to reload with more than magazine capacity!");
    Ammo += Amount;
}

bool AWeapon::ClipIsFull() const { return Ammo >= MagazineCapacity; }

void AWeapon::StartSlideTimer() {
    bMovingSlide = true;

    GetWorldTimerManager().SetTimer(
      SlideTimer, this, &AWeapon::FinishMovingSlide, SlideDisplacementTime);
}
