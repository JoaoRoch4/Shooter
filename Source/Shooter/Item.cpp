#include "Item.h"

#include "Custom.h"
#include "ShooterCharacter.h"

#include <Camera/CameraComponent.h>
#include <Components/BoxComponent.h>
#include <Components/SphereComponent.h>
#include <Components/WidgetComponent.h>
#include <Curves/CurveVector.h>
#include <Engine/Engine.h>
#include <Kismet\GameplayStatics.h>
#include <Sound/SoundCue.h>

AItem::AItem()
 :

 ItemMesh(nullptr)
 , CollisionBox(nullptr)
 , PickupWidget(nullptr)
 , AreaSphere(nullptr)
 , ItemName(FString(L"Default"))
 , ItemCount(0)
 , ItemRarity(EItemRarity::EIR_Common)
 , ItemState(EItemState::EIS_Pickup)
 , ActiveStars(TArray<bool>())
 , ItemZ_Curve(nullptr)
 , ItemInterpStartLocation(FVector(0.f))
 , CameraTargetLocation(FVector(0.f))
 , bInterping(false)
 , ItemInterpTimer(FTimerHandle())
 , Z_CurveTime(0.7f)
 , Character(nullptr)
 , ItemInterpX(0.f)
 , ItemInterpY(0.f)
 , InterpInitialYawOffset(0.f)
 , ItemScaleCurve(nullptr)
 , PickupSound(nullptr)
 , EquipSound(nullptr)
 , ItemType(EItemType::EIT_MAX)
 , InterpLocIndex(0)
 , MaterialIndex(0)
 , DynamicMaterialInstance(nullptr)
 , MaterialInstance(nullptr)
 , bCanChangeCustomDepth(true)
 , PulseCurve(nullptr)
 , InterpPulseCurve(nullptr)
 , PulseTimer(FTimerHandle())
 , GlowAmount(150.f)
 , FresnelExponent(3.f)
 , FresnelReflectFraction(4.f)
 , PulseCurveTime(5.f)
 , IconBackground(nullptr)
 , IconItem(nullptr) {

    PrimaryActorTick.bCanEverTick = true;

    ItemMesh = CDSubObj<USkeletalMeshComponent>(L"Item Mesh");
    SetRootComponent(ItemMesh);

    DefaultConstructor_CollisionBox();

    PickupWidget = CDSubObj<UWidgetComponent>(L"Pickup Widget");
    PickupWidget->SetupAttachment(GetRootComponent());

    AreaSphere = CDSubObj<USphereComponent>(L"Area Sphere");
    AreaSphere->SetupAttachment(GetRootComponent());

    DynamicMaterialInstance = CDSubObj<UMaterialInstanceDynamic>(L"DynamicMaterialInstance");


    DefaultConstructor_Curves();
}

void AItem::BeginPlay() {

    Super::BeginPlay();

    // hide the pickup widget
    if (PickupWidget) PickupWidget->SetVisibility(false);

    // Sets active stars array based on rarity
    SetActiveStars();

    // Setup overlap for the area sphere
    AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
    AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

    // Set Properties of itens components based on state
    SetItemProperties(ItemState);

    // Set custom depth to disabled
    InitializeCustomDepth();

    StartPulseTimer();
}

void AItem::OnConstruction(const FTransform &Transform) {


    if (MaterialInstance) {

        DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialInstance, this);

        ItemMesh->SetMaterial(MaterialIndex, DynamicMaterialInstance);

    } else {

        PrintLogErr("AItem::OnConstruction(const FTransform &Transform): "
                    "MaterialInstance was nullptr");
    }

    EnableGlowMaterial();
}

void AItem::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);

    // Handle item interpolation when in the EquipInterping state
    ItemInterp(DeltaTime);

    // Get curve values from PulseCurve and set dynamic material parameters
    UpdatePulse();
}

void AItem::DefaultConstructor_CollisionBox() {

    CollisionBox = CDSubObj<UBoxComponent>(L"CollisionBox");

    CollisionBox->SetupAttachment(ItemMesh);

    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    CollisionBox->SetCollisionResponseToChannel(
      ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AItem::DefaultConstructor_Curves() {

    // PulseCurve
    {
        PulseCurve = CDSubObj<UCurveVector>(L"PulseCurve");

        const static ConstructorHelpers::FObjectFinder<UCurveVector> M_PulseCurve(
          L"/Script/Engine.CurveVector'/Game/_Game/Assets/Curves/"
          L"MaterialPulseCurve.MaterialPulseCurve'");

        if (M_PulseCurve.Succeeded()) {
            PulseCurve = M_PulseCurve.Object;
        } else {
            PrintLogErr("AItem::DefaultConstructor_Curves(): M_PulseCurve "
                        "failed to load");
        }
    }

    // InterpPulseCurve
    {
        InterpPulseCurve = CDSubObj<UCurveVector>(L"InterpPulseCurve");

        const static ConstructorHelpers::FObjectFinder<UCurveVector> M_InterpPulseCurve(
          L"/Script/Engine.CurveVector'/Game/_Game/Assets/"
          L"Curves/MaterialInterpCurve.MaterialInterpCurve'");

        if (M_InterpPulseCurve.Succeeded()) {
            InterpPulseCurve = M_InterpPulseCurve.Object;
        } else {
            PrintLogErr("AItem::DefaultConstructor_Curves(): M_InterpPulseCurve "
                        "failed to load");
        }
    }
}

FVector AItem::GetInterpLocation() {

    if (Character == nullptr) return FVector::Zero();

    switch (ItemType) {

        case EItemType::EIT_Ammo :
            {

                return Character->GetInterpLocation(InterpLocIndex).SceneComponent->GetComponentLocation();
                break;
            }
        case EItemType::EIT_Weapon :
            {

                return Character->GetInterpLocation(0).SceneComponent->GetComponentLocation();
                break;
            }
    }
    return FVector::Zero();
}

void AItem::PlayPickupSound() {

    if (Character) {

        if (Character->ShouldPlayPickupSound()) {

            Character->StartPickupSoundTimer();

            if (PickupSound) UGameplayStatics::PlaySound2D(this, PickupSound);
        }
    } else {
        PrintLogErr("AItem::PlayPickupSound(): Character was nullptr");
    }
}

void AItem::EnableCustomDepth() {

    if (bCanChangeCustomDepth) ItemMesh->SetRenderCustomDepth(true);
}

void AItem::DisableCustomDepth() {

    if (bCanChangeCustomDepth) ItemMesh->SetRenderCustomDepth(false);
}

void AItem::InitializeCustomDepth() { DisableCustomDepth(); }

void AItem::StartPulseTimer() {

    if (ItemState == EItemState::EIS_Pickup) {
        GetWorldTimerManager().SetTimer(PulseTimer, this, &AItem::ResetPulseTimer, PulseCurveTime);
    }
}

void AItem::ResetPulseTimer() { StartPulseTimer(); }

void AItem::UpdatePulse() {

    float   ElapsedTime {0.f};
    FVector CurveValue {FVector()};

    switch (ItemState) {

        case EItemState::EIS_Pickup :
            {
                if (PulseCurve) {

                    ElapsedTime = GetWorldTimerManager().GetTimerElapsed(PulseTimer);
                    CurveValue  = PulseCurve->GetVectorValue(ElapsedTime);

                } else {
                    PrintLogErr("AItem::UpdatePulse(): Case EItemState::EIS_Pickup: "
                                "PulseCurve was nullptr");
                }
                break;
            }
        case EItemState::EIS_EquipInterping :
            {

                if (InterpPulseCurve) {

                    ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer);
                    CurveValue  = InterpPulseCurve->GetVectorValue(ElapsedTime);

                } else {
                    PrintLogErr("AItem::UpdatePulse(): Case "
                                "EItemState::EIS_EquipInterping: "
                                "InterpPulseCurve was nullptr");
                }
                break;
            }
    }

    if (DynamicMaterialInstance) {

        DynamicMaterialInstance->SetScalarParameterValue(
          FName(TEXT("GlowAmount")), CurveValue.X * GlowAmount);

        DynamicMaterialInstance->SetScalarParameterValue(
          FName(TEXT("FresnelExponent")), CurveValue.Y * FresnelExponent);

        DynamicMaterialInstance->SetScalarParameterValue(
          FName(TEXT("FresnelReflectFraction")), CurveValue.Z * FresnelReflectFraction);
    } else {

        PrintLogErr("AItem::UpdatePulse(): DynamicMaterialInstance was nullptr");
        PrintOnScrTime(0, "DynamicMaterialInstance nullptr!");
    }
}

void AItem::EnableGlowMaterial() {

    if (DynamicMaterialInstance) {

        DynamicMaterialInstance->SetScalarParameterValue(FName(TEXT("GlowBlendAlpha")), 0.f);
    } else {

        PrintLogErr("AItem::EnableGlowMaterial(): DynamicMaterialInstance was nullptr");
    }
}

void AItem::DisableGlowMaterial() {

    if (DynamicMaterialInstance) {

        DynamicMaterialInstance->SetScalarParameterValue(FName(TEXT("GlowBlendAlpha")), 1.f);

    } else {

        PrintLogErr("AItem::DisableGlowMaterial(): DynamicMaterialInstance was nullptr");
    }
}

void AItem::PlayEquipSound() {

    if (Character) {

        if (Character->ShouldPlayEquipSound()) {

            Character->StartEquipSoundTimer();

            if (EquipSound) UGameplayStatics::PlaySound2D(this, EquipSound);
        }
    } else {
        PrintLogErr("AItem::PlayEquipSound(): Character was nullptr");
    }
}

void AItem::SetItemState(EItemState State) {

    ItemState = State;
    SetItemProperties(State);
}

void AItem::StartItemCurve(AShooterCharacter *Char) {

    // Store a ref to the character
    Character = Char;

    // Get array index in InterpLocations with the lowest item count
    InterpLocIndex = Character->GetInterpLocationIndex();

    // Add 1 to the item count in this Location Struct
    Character->IncrementInterpLocItemCount(InterpLocIndex, 1);

    PlayPickupSound();

    // Store initial location of the item
    ItemInterpStartLocation = GetActorLocation();

    bInterping = true;
    SetItemState(EItemState::EIS_EquipInterping);
    GetWorldTimerManager().ClearTimer(PulseTimer);

    GetWorldTimerManager().SetTimer(ItemInterpTimer, this, &AItem::FinishInterping, Z_CurveTime);

    double GetCameraRotationYaw {Character->GetFollowCamera()->GetComponentRotation().Yaw};
    double GetItemRotationYaw {GetActorRotation().Yaw};

    // Get initial yaw of the camera
    float CameraRotationYaw {StaticCast<float>(GetCameraRotationYaw)};

    // Get initial yaw of the item
    float ItemRotationYaw {StaticCast<float>(GetItemRotationYaw)};

    // Initial yaw offset between camera and item
    InterpInitialYawOffset = ItemRotationYaw - CameraRotationYaw;

    bCanChangeCustomDepth = false;
}

void AItem::OnSphereOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) {

    if (OtherActor) {

        AShooterCharacter *ShooterCharacter = Cast<AShooterCharacter>(OtherActor);

        if (ShooterCharacter) {

            ShooterCharacter->IncrementOverlappedItemCount(1);
        }
    }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {

    if (OtherActor) {

        AShooterCharacter *ShooterCharacter = Cast<AShooterCharacter>(OtherActor);

        if (ShooterCharacter) {

            ShooterCharacter->IncrementOverlappedItemCount(-1);
        }
    }
}

void AItem::SetActiveStars() {

    // The 0 element isn't used
    for (int32 i = 0; i <= 5; i++) {
        ActiveStars.Add(false);
    }

    switch (ItemRarity) {

        case EItemRarity::EIR_Damege : ActiveStars [1] = true; break;

        case EItemRarity::EIR_Common :
            for (int32 i = 1; i <= 2; i++)
                ActiveStars [i] = true;
            break;

        case EItemRarity::EIR_Uncommon :
            for (int32 i = 1; i <= 3; i++)
                ActiveStars [i] = true;
            break;

        case EItemRarity::EIR_Rare :
            for (int32 i = 1; i <= 4; i++)
                ActiveStars [i] = true;
            break;

        case EItemRarity::EIR_Legendary :
            for (int32 i = 1; i <= 5; i++)
                ActiveStars [i] = true;
            break;
    }
}

void AItem::SetItemProperties(EItemState State) {

    switch (State) {

        case EItemState::EIS_Pickup :
            // Set mesh properties
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->SetEnableGravity(false);
            ItemMesh->SetVisibility(true);
            ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set AreaSphere properties
            AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
            AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            // Set CollisionBox properties
            CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            CollisionBox->SetCollisionResponseToChannel(
              ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
            CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            break;

        case EItemState::EIS_Equipped :

            PickupWidget->SetVisibility(false);
            // Set mesh properties
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->SetEnableGravity(false);
            ItemMesh->SetVisibility(true);
            ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set AreaSphere properties
            AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set CollisionBox properties
            CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            break;

        case EItemState::EIS_Falling :

            // Set mesh properties
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            ItemMesh->SetSimulatePhysics(true);
            ItemMesh->SetEnableGravity(true);
            ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
            ItemMesh->SetCollisionResponseToChannel(
              ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
            // Set AreaSphere properties
            AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            // Set CollisionBox properties
            CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            break;

        case EItemState::EIS_EquipInterping :

            PickupWidget->SetVisibility(false);
            // Set mesh properties
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->SetEnableGravity(false);
            ItemMesh->SetVisibility(true);
            ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set AreaSphere properties
            AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set CollisionBox properties
            CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            break;

        case EItemState::EIS_PickedUp :

            PickupWidget->SetVisibility(false);
            //Set mesh properties
            ItemMesh->SetSimulatePhysics(false);
            ItemMesh->SetEnableGravity(false);
            ItemMesh->SetVisibility(false);
            ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set AreaSphere properties
            AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // Set CollisionBox properties
            CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);            
            break;
    }
}

void AItem::FinishInterping() {

    bInterping = false;

    if (Character) {

        // Subtract 1 from the item count in this location struct
        Character->IncrementInterpLocItemCount(InterpLocIndex, -1);

        Character->GetPickupItem(this);
        SetItemState(EItemState::EIS_PickedUp);

    } else {
        PrintLogErr("AItem::FinishInterping(): Character was nullptr");
    }
    // Set item state back to normal
    SetActorScale3D(FVector(1.f));

    DisableGlowMaterial();
    bCanChangeCustomDepth = true;
    DisableCustomDepth();
}

void AItem::ItemInterp(float DeltaTime) {

    if (!bInterping) return;

    if (Character && ItemZ_Curve) {

        // Elapsed time since we started ItemInterpTimer
        const float ElapsedTime {GetWorldTimerManager().GetTimerElapsed(ItemInterpTimer)};

        // Get curve value corresponding to ElapsedTime
        const float CurveValue {ItemZ_Curve->GetFloatValue(ElapsedTime)};

        // UE_LOG(LogTemp, Warning, TEXT("CurveValue: %f"), CurveValue);

        // Get the item's initial location when the curve started
        FVector ItemLocation {ItemInterpStartLocation};

        // Get location in front of the camera
        const FVector CameraInterpLocation {GetInterpLocation()};

        // Vector from Item to Camera Interp Location, X and Y are zeroed out
        const FVector ItemToCamera {FVector(0.f, 0.f, (CameraInterpLocation - ItemLocation).Z)};

        const double GetDeltaZ {ItemToCamera.Size()};

        // Scale factor to multiply with CurveValue
        const float DeltaZ {StaticCast<const float>(GetDeltaZ)};

        const FVector CurrentLocation {GetActorLocation()};

        // Interp speed: 30 units per second
        float InterpSpeed {30.f};

        const double GetInterpX_Value {
          FMath::FInterpTo(CurrentLocation.X, CameraInterpLocation.X, DeltaTime, InterpSpeed)};

        const double GetInterpY_Value {
          FMath::FInterpTo(CurrentLocation.Y, CameraInterpLocation.Y, DeltaTime, InterpSpeed)};

        // Interpolated X values
        const float InterpX_Value {StaticCast<const float>(GetInterpX_Value)};

        // Interpolated X values
        const float InterpY_Value {StaticCast<const float>(GetInterpY_Value)};

        // Set X and Y of ItemLocation to interp values
        ItemLocation.X = InterpX_Value;
        ItemLocation.Y = InterpY_Value;

        // Adding curve value to the Z component of the Initial Location (scaled
        // by DeltaZ)
        ItemLocation.Z += CurveValue * DeltaZ;

        SetActorLocation(ItemLocation, true, nullptr, ETeleportType::TeleportPhysics);

        // Camera Rotation this frame
        const FRotator CameraRotation {Character->GetFollowCamera()->GetComponentRotation()};

        // Camera Rotation + Initial Yaw Offset
        FRotator ItemRotation {0.f, CameraRotation.Yaw + InterpInitialYawOffset, 0.f};

        SetActorRotation(ItemRotation, ETeleportType::TeleportPhysics);

        if (ItemScaleCurve) {

            const float ScaleCurveValue {ItemScaleCurve->GetFloatValue(ElapsedTime)};

            SetActorScale3D(FVector(ScaleCurveValue, ScaleCurveValue, ScaleCurveValue));
        } else {
            PrintLogErr("AItem::ItemInterp(float DeltaTime):->if(Character && "
                        "ItemZ_Curve): ItemScaleCurve was nullptr");
        }
    } else {
        PrintLogErr("AItem::ItemInterp(float DeltaTime):->if(Character && "
                    "ItemZ_Curve): Character or ItemZ_Curve was nullptr");
    }
}
