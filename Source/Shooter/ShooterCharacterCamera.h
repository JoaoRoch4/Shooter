#pragma once

#include <Camera/CameraComponent.h>

#include "CoreMinimal.h"

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|My Custom Camera|FOV",
      meta = (AllowPrivateAccess = "true"))
    float fDefaultFOV;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Category = "My Custom Properties|My Custom Camera|",
      meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CustomCameraBoom;

public:

    void SetFOV(float FOV);

    float GetFOV() const;

    USpringArmComponent *GetCustomCameraBoom() const;
};
