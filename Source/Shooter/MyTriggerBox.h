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

    UPROPERTY()
    class ADebugSounds *DebugSounds;

    UPROPERTY()
    FTimerHandle TimerHandle;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|CoolDown", meta = (AllowPrivateAccess = "true"))
    float CoolDown;

    UFUNCTION(BlueprintCallable)
    void CoolDownOver();

    bool bCanOverlap;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|ExitGame", meta = (AllowPrivateAccess = "true"))
    bool bExitGame;

};
