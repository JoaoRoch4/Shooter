#include "DebugSounds.h"

#include <Sound/SoundCue.h>

ADebugSounds::ADebugSounds()
 : BeginOverlapSound(nullptr)
 , EndOverlapSound(nullptr)
 , NullptrSound(nullptr)
 , CustomSound(nullptr) {

    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADebugSounds::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADebugSounds::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
