#include "Ammo.h"

#include "Custom.h"
#include "ErrorHandling.h"
#include "ShooterCharacter.h"

#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include "AmmoType.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Delegates/Delegate.h"
#include "Engine/EngineTypes.h"
#include "Engine/HitResult.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "Item.h"
#include "Templates/Casts.h"

AAmmo::AAmmo()
 : AmmoMesh(nullptr)
 , AmmoType(EAmmoType::EAT_9mm)
 , AmmoIconTexture(nullptr)
 , AmmoCollisionSphere(nullptr) {

    // Construct the AmmoMesh component and set it as the root component.
    AmmoMesh = CDSubObj<UStaticMeshComponent>(L"AmmoMesh");
    SetRootComponent(AmmoMesh);

    GetAreaSphere()->SetupAttachment(GetRootComponent());
    GetCollisionBox()->SetupAttachment(GetRootComponent());
    GetPickupWidget()->SetupAttachment(GetRootComponent());

    AmmoCollisionSphere = CDSubObj<USphereComponent>(L"AmmoCollisionSphere");
    AmmoCollisionSphere->SetupAttachment(GetRootComponent());
    AmmoCollisionSphere->SetSphereRadius(50.0f);
}

void AAmmo::BeginPlay() {

    Super::BeginPlay();
    AmmoCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::AmmoSphereOverlap);
}

void AAmmo::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AAmmo::SetItemProperties(EItemState State) {

    Super::SetItemProperties(State);

    switch (State) {

        case EItemState::EIS_Pickup : {

            // Set mesh properties
            AmmoMesh->SetSimulatePhysics(false);
            AmmoMesh->SetEnableGravity(false);
            AmmoMesh->SetVisibility(true);
            AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        } break;

        case EItemState::EIS_Equipped : {

            // Set mesh properties
            AmmoMesh->SetSimulatePhysics(false);
            AmmoMesh->SetEnableGravity(false);
            AmmoMesh->SetVisibility(true);
            AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        } break;

        case EItemState::EIS_Falling : {

            // Set mesh properties
            AmmoMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            AmmoMesh->SetSimulatePhysics(true);
            AmmoMesh->SetEnableGravity(true);
            AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AmmoMesh->SetCollisionResponseToChannel(
              ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

        } break;

        case EItemState::EIS_EquipInterping : {

            // Set mesh properties
            AmmoMesh->SetSimulatePhysics(false);
            AmmoMesh->SetEnableGravity(false);
            AmmoMesh->SetVisibility(true);
            AmmoMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AmmoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        } break;
    }
}

void AAmmo::AmmoSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
  const FHitResult &SweepResult) {

    CheckPtr(OtherActor);

    AShooterCharacter *OverlappedCharacter {Cast<AShooterCharacter>(OtherActor)};

    CheckPtr(OverlappedCharacter);

    StartItemCurve(OverlappedCharacter);
    AmmoCollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAmmo::EnableCustomDepth() { AmmoMesh->SetRenderCustomDepth(true); }

void AAmmo::DisableCustomDepth() { AmmoMesh->SetRenderCustomDepth(false); }
