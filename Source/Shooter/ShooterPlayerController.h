// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

using namespace UP;

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    AShooterPlayerController();

protected:
    virtual void BeginPlay() override;

private:
    /** 
	 * @brief Reference to the Overall HUD Overlay
	 * Blueprint
     * class. 
	 */
    UPROPERTY(EditAnywhere,
        BlueprintReadWrite,
        Category = "Widgets",
        meta     = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> HUDOverlayClass;

    /** 
     * @brief Variable to hold the HUD Overlay
     * during gameplay
     * after creating it. 
     */
    UPROPERTY(VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Widgets",
        meta     = (AllowPrivateAccess = "true"))
    UUserWidget *HUDOverlay;
};
