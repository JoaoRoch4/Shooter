#include "Weapon.h"

#include "Custom.h"
#include "Item.h"

#include <Engine\Engine.h>
#include <Engine\World.h>
#include <Kismet\GameplayStatics.h>

AWeapon::AWeapon() :

    ThrowWeaponTime(0.7f),
    bFalling(false),
    bNotRandValues(false),
    AngleRotation(10.f),
    AddAdiotionalRotation(90.f),
    MultiplyImpulse(1'800.f),
    AddImpulseDirectionRandRotation(FVector2D(0.f, 90.f)),
    MultiplyImpulseRandRange(FVector2D(1'500.f, 1'800.f)),
    ThrowWeaponTimer(FTimerHandle()),
    Ammo(30),
    MagazineCapacity(30),
    WeaponType(EWeaponType::EWT_SubmachineGun),
    AmmoType(EAmmoType::EAT_9mm),
    ReloadMontageSection(FName(L"Reload SMG")),
    ClipBoneName(L"smg_clip"),
    ItemInstance(nullptr)

{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay() {

    Super::BeginPlay();

    //SyncItemMunition();
}

void AWeapon::SyncItemMunition() {

    int32 ItemCapacity { 0 };

    //ItemInstance = Cast<AItem>(UGameplayStatics::GetActorOfClass(
        //GetWorld(), AWeapon::StaticClass()));

    if (ItemInstance) {

        ItemCapacity = ItemInstance->GetItemCount();

        if ((Ammo != ItemCapacity)) {

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

        const FRotator MeshRotation { 0.f,
            GetItemMesh()->GetComponentRotation().Yaw, 0.f };

        GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr,
            ETeleportType::TeleportPhysics);
    }
}

void AWeapon::ThrowWeapon() {

    FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };

	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
	const FVector MeshRight{ GetItemMesh()->GetRightVector() };

	// Direction in which we throw the Weapon
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(AngleRotation, MeshForward);

    float RandomRotation = (!bNotRandValues) ?

        FMath::RandRange(
        AddImpulseDirectionRandRotation.X,
        AddImpulseDirectionRandRotation.Y) :

        AddAdiotionalRotation;

    ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation,
        FVector(0.f, 0.f, 1.f));
       

    if(bNotRandValues){ 

        MultiplyImpulse *= 1000.f;
        ImpulseDirection *= MultiplyImpulse;

    } else {

        MultiplyImpulseRandRange * 1000.f;
        ImpulseDirection *= FMath::RandRange(
			MultiplyImpulseRandRange.X,
			MultiplyImpulseRandRange.Y);
    }                

	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;

	GetWorldTimerManager().SetTimer(
		ThrowWeaponTimer, 
		this, 
		&AWeapon::StopFalling, 
		ThrowWeaponTime);

    EnableGlowMaterial();
}

void AWeapon::StopFalling() {

    bFalling = false;

    SetItemState(EItemState::EIS_Pickup);

    StartPulseTimer();
}

void AWeapon::DecrementAmmo() {

    if (Ammo - 1 <= 0) {

        Ammo = 0;

    } else {

        --Ammo;
    }
}

void AWeapon::ReloadAmmo(int32 Amount) {

    checkf(Ammo + Amount <= MagazineCapacity, L"Attempted to reload with more than magazine capacity!");
    Ammo += Amount;
}

inline bool AWeapon::ClipIsFull() const {

    return Ammo >= MagazineCapacity;
}


