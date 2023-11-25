
#include "ShooterCharacterCamera.h"

UShooterCharacterCamera::UShooterCharacterCamera() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UShooterCharacterCamera::BeginPlay() { Super::BeginPlay(); }

void UShooterCharacterCamera::TickComponent(
  float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
