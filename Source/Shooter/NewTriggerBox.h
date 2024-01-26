// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "NewTriggerBox.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API ANewTriggerBox : public ATriggerBox {

    GENERATED_BODY()

protected:

    virtual void BeginPlay() override;

public:

    ANewTriggerBox();

    // declare overlap begin function
    UFUNCTION()
    void OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor);

    UFUNCTION()
    virtual void BeginOverlap(class AActor *OverlappedActor = nullptr, class AActor *OtherActor = nullptr);

    // declare overlap end function
    UFUNCTION()
    void OnOverlapEnd(class AActor *OverlappedActor, class AActor *OtherActor);

    UFUNCTION()
    virtual void EndOverlap(class AActor *OverlappedActor = nullptr, class AActor *OtherActor = nullptr);

     UFUNCTION(BlueprintCallable)
    inline void CoolDownOver() { bCanOverlap = true; };

    private:

    UPROPERTY()
    FTimerHandle TimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Custom Properties|CoolDown",
      meta = (AllowPrivateAccess = "true"))
    float CoolDown;

    bool bCanOverlap;
};
