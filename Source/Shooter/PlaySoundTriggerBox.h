#pragma once

#include "CoreMinimal.h"
#include "NewTriggerBox.h"
#include "PlaySoundTriggerBox.generated.h"

/**
 *
 */
UCLASS()
class SHOOTER_API APlaySoundTriggerBox : public ANewTriggerBox {
    GENERATED_BODY()

public:

    APlaySoundTriggerBox();

protected:

    virtual void BeginPlay() override;

    virtual void BeginOverlap(
      class AActor *OverlappedActor = nullptr, class AActor *OtherActor = nullptr) override;

    virtual void EndOverlap(
      class AActor *OverlappedActor = nullptr, class AActor *OtherActor = nullptr) override;

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",
      meta = (AllowPrivateAccess = "true"))
    class UAudioComponent *AudioComponent;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    class USoundCue *BeginOverlapSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
    USoundCue *EndOverlapSound;
};
