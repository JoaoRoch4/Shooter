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

public:

    AMyTriggerBox();

    

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor);

    UFUNCTION()
    void OnOverlapEnd(class AActor *OverlappedActor, class AActor *OtherActor);

    void OverlapBeginHappened();
    void OverlapEndHappened();
    
private:

    UPROPERTY()
    class AShooterCharacter *ShooterCharacter;

};
