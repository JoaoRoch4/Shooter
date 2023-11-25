// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ShooterCharacterCamera.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API UShooterCharacterCamera : public UCameraComponent {
    GENERATED_BODY()

public:
    UShooterCharacterCamera();

protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
