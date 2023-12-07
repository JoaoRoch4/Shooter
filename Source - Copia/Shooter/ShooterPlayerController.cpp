#include "ShooterPlayerController.h"

#include <Blueprint\UserWidget.h>

AShooterPlayerController::AShooterPlayerController()

  : HUDOverlay(nullptr) {}

void AShooterPlayerController::BeginPlay() {

    Super::BeginPlay();

    // Check our HUDOverlayClass TSubClassOf variable
    if (HUDOverlayClass) {

	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);

	if (HUDOverlay) {

	    HUDOverlay->AddToViewport();
	    HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
    }
}
