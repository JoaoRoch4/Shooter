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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|Camera|FOV",
      meta = (AllowPrivateAccess = "true"))
    float fDefaultFOV;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Custom Properties|Camera",
      meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent *CustomCameraBoom;

public:

    FORCEINLINE void SetFOV(float FOV);

    FORCEINLINE float GetFOV() const;

    FORCEINLINE USpringArmComponent *GetCustomCameraBoom() const;
};
