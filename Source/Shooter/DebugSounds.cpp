#include "DebugSounds.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

ADebugSounds::ADebugSounds()
 : BeginOverlapSound(nullptr)
 , EndOverlapSound(nullptr)
 , NullptrSound(nullptr)
 , CustomSound_1(nullptr) {

    PrimaryActorTick.bCanEverTick = true;

    BeginOverlapSound = CDSubObj<USoundCue>(L"BeginOverlapSound");
    EndOverlapSound   = CDSubObj<USoundCue>(L"EndOverlapSound");
    NullptrSound      = CDSubObj<USoundCue>(L"NullptrSound");
    CustomSound_1     = CDSubObj<USoundCue>(L"CustomSound_1");
}

void ADebugSounds::BeginPlay() { Super::BeginPlay(); }

void ADebugSounds::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADebugSounds::PlayBeginOverlapSound() {

    if (BeginOverlapSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), BeginOverlapSound);
    } else {
        PrintOnScr("BeginOverlapSound is nullptr");
    }
}

void ADebugSounds::PlayEndOverlapSound() {

    if (EndOverlapSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), EndOverlapSound);
    } else {
        PrintOnScr("EndOverlapSound is nullptr");
    }
}

void ADebugSounds::PlayNullptrSound() {

    if (NullptrSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), NullptrSound);
    } else {
        PrintOnScr("NullptrSound is nullptr");
    }
}

void ADebugSounds::PlayCustomSound_1() {

    if (CustomSound_1) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_1);
    } else {
        PrintOnScr("CustomSound_1 is nullptr");
    }
}
