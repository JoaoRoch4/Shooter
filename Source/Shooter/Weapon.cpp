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
	ThrowHeight(10.f),
	AddAdiotionalRotation(120.f),
	MultiplyImpulse(1'800.f),
	ThrowHeightRandRange(FVector2D(90.f, 180.f)),
	AddAdiotionalRotationRandRange(FVector2D(90.f, 180.f)),
	MultiplyImpulseRandRange(FVector2D(1500.f, 1800.f)),
	ThrowWeaponTimer(FTimerHandle()),
	Ammo(40),
	MagazineCapacity(120),
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
	FVector ImpulseDirection {};
		
	if (bNotRandValues) {

		ImpulseDirection = MeshForward.RotateAngleAxis(ThrowHeight * -1.f, MeshRight);

	} else {

		double RandThrowHeight = FMath::RandRange(
			ThrowHeightRandRange.X,
			ThrowHeightRandRange.Y);

		RandThrowHeight *= -1.f;

		ImpulseDirection = MeshForward.RotateAngleAxis(RandThrowHeight,
			MeshRight);
	}

	float RandAddAdiotionalRotation = FMath::RandRange(
		AddAdiotionalRotationRandRange.X,
		AddAdiotionalRotationRandRange.Y);

	float RandomRotation = bNotRandValues ? AddAdiotionalRotation : RandAddAdiotionalRotation;

	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation,
		FVector(0.f, 0.f, 1.f));	   

	if(bNotRandValues){ 

		MultiplyImpulse *= 1000.f;
		ImpulseDirection *= MultiplyImpulse;

	} else {

		MultiplyImpulseRandRange *= 1000.f;
		double RandMultiplyImpulse = FMath::RandRange(
			MultiplyImpulseRandRange.X,
			MultiplyImpulseRandRange.Y);

		ImpulseDirection *= RandMultiplyImpulse;
	}                

	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;

	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this,
		&AWeapon::StopFalling, ThrowWeaponTime);

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


