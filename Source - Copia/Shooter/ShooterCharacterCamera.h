// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Custom.h"
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

    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
      FActorComponentTickFunction *ThisTickFunction) override;

private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|FOV",
      meta = (AllowPrivateAccess = "true"))
    float fDefaultFOV;

public:

    FORCEINLINE void SetFOV(float FOV);

    FORCEINLINE float GetFOV() const;
};
