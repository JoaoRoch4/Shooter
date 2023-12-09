#include "ShooterCharacterCamera.h"

#include <GameFramework/SpringArmComponent.h>

UShooterCharacterCamera::UShooterCharacterCamera()
    : fDefaultFOV(110.f)
    , CustomCameraBoom (nullptr){

    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.bCanEverTick          = true;
    bUsePawnControlRotation                    = false;

    //CustomCameraBoom = CDSubObj<USpringArmComponent>(L"CustomCameraBoom");

    SetFieldOfView(fDefaultFOV);
}

void UShooterCharacterCamera::BeginPlay() { Super::BeginPlay(); }

void UShooterCharacterCamera::TickComponent(
  float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShooterCharacterCamera::SetFOV(float FOV) {}

float UShooterCharacterCamera::GetFOV() const { return fDefaultFOV; }

USpringArmComponent *UShooterCharacterCamera::GetCustomCameraBoom() const {
    return CustomCameraBoom;
}
