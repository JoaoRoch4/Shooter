#pragma once

#include "CoreMinimal.h"
#include "Custom.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

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

    void OverlapBeginHappened(AActor *OverlappedActor = nullptr, AActor *OtherActor = nullptr);
    void OverlapEndHappened(AActor *OverlappedActor = nullptr, AActor *OtherActor = nullptr);

private:

    UPROPERTY()
    class ADebugSounds *DebugSounds;

    UPROPERTY()
    FTimerHandle TimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|CoolDown",
      meta = (AllowPrivateAccess = "true"))
    float CoolDown;

    UFUNCTION(BlueprintCallable)
    void CoolDownOver();

    bool bCanOverlap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|ExitGame",
      meta = (AllowPrivateAccess = "true"))
    bool bExitGame;

    class ACustom *Custom;

    class ADefaultRandom *DefaultRandom;
};
