#pragma once

#include "CoreMinimal.h"
#include "Custom.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

using namespace UnrealBasic;

/**
 *
 */
UCLASS()
class SHOOTER_API AMyTriggerBox : public ATriggerBox {

    GENERATED_BODY()


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    void SetTriggerBoxProperties();

public:
    AMyTriggerBox();

    void BeginOverlapHapped();
    void EndOverlapHapped();
    
private:

    class UBoxComponent *TriggerBox;
    class AShooterCharacter *ShooterCharacter;


    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
      class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
      const FHitResult &SweepResult);

    // declare overlap end function
    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
      class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);


};
