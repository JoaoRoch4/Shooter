
#include "ShooterCharacterCamera.h"

UShooterCharacterCamera::UShooterCharacterCamera() : fDefaultFOV(110.f) {

    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.bCanEverTick = true;
    bUsePawnControlRotation = false;

    SetFieldOfView(fDefaultFOV);
}

void UShooterCharacterCamera::BeginPlay() { Super::BeginPlay(); }

void UShooterCharacterCamera::TickComponent(
  float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShooterCharacterCamera::SetFOV(float FOV) {}

float UShooterCharacterCamera::GetFOV() const { return fDefaultFOV; }
