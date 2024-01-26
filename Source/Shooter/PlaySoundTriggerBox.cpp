#include "PlaySoundTriggerBox.h"

#include "Custom.h"

#include <Components/AudioComponent.h>
#include <DrawDebugHelpers.h>
#include "Sound/SoundCue.h"

APlaySoundTriggerBox::APlaySoundTriggerBox()
 : AudioComponent(nullptr)
 , BeginOverlapSound(nullptr)
 , EndOverlapSound(nullptr) {

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);
    AudioComponent->bAutoActivate = false;
}

void APlaySoundTriggerBox::BeginPlay() {

    ANewTriggerBox::BeginPlay();

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Yellow, true, -1, 0, 5);
}

void APlaySoundTriggerBox::BeginOverlap(AActor *OverlappedActor, AActor *OtherActor) {

    CLS();
    PrintOnScr("PlaySoundTriggerBox::BeginOverlap");

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Emerald, true, -1, 0, 5);

    if (BeginOverlapSound) {

        if (AudioComponent && BeginOverlapSound) {
            AudioComponent->SetSound(BeginOverlapSound);
            AudioComponent->Play();
        }
    }
}

void APlaySoundTriggerBox::EndOverlap(AActor *OverlappedActor, AActor *OtherActor) {

    CLS();
    PrintOnScr("PlaySoundTriggerBox::EndOverlap");

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Yellow, true, -1, 0, 5);

    if (EndOverlapSound) {

        if (AudioComponent && EndOverlapSound) {
            AudioComponent->SetSound(EndOverlapSound);
            AudioComponent->Play();
        }
    }
}
