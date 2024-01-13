#include "DebugSounds.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>
#include <Templates/UniquePtr.h>

ADebugSounds::ADebugSounds()
 : BeginOverlapSound(nullptr)
 , EndOverlapSound(nullptr)
 , NullptrSound(nullptr)
 , CustomSound_1(nullptr)
 , CustomSound_2(nullptr)
 , CustomSound_3(nullptr)
 , CustomSound_4(nullptr)
 , CustomSound_5(nullptr)
 , BeginOverlapSoundPath(FString())
 , EndOverlapSoundPath(FString())
 , NullptrSoundPath(FString())
 , BlankCue(FString())
 , M_BeginOverlapSoundPath(nullptr)
 , M_EndOverlapSoundPath(nullptr)
 , M_NullptrSoundPath(nullptr)
 , M_BlankCue(nullptr) {
    PrimaryActorTick.bCanEverTick = true;

    BeginOverlapSound = CreateDefaultSubobject<USoundCue>(L"BeginOverlapSound");
    EndOverlapSound   = CreateDefaultSubobject<USoundCue>(L"EndOverlapSound");
    NullptrSound      = CreateDefaultSubobject<USoundCue>(L"NullptrSound");
    CustomSound_1     = CreateDefaultSubobject<USoundCue>(L"CustomSound_1");
    CustomSound_2     = CreateDefaultSubobject<USoundCue>(L"CustomSound_2");
    CustomSound_3     = CreateDefaultSubobject<USoundCue>(L"CustomSound_3");
    CustomSound_4     = CreateDefaultSubobject<USoundCue>(L"CustomSound_4");
    CustomSound_5     = CreateDefaultSubobject<USoundCue>(L"CustomSound_5");

    //SetSounds();
}

void ADebugSounds::SetSounds() {

    BeginOverlapSoundPath
      = FString(L"/Script/Engine.SoundCue'"
                L"/Game/_Game/Assets/Sounds/Interfaces/button-124476_Cue.button-124476_Cue'");

    EndOverlapSoundPath
      = FString(L"/Script/Engine.SoundCue'"
                L"/Game/_Game/Assets/Sounds/Interfaces/interface-124464_Cue.interface-124464_Cue'");

    NullptrSoundPath = FString(
      L"/Script/Engine.SoundCue'"
      L"/Game/_Game/Assets/Sounds/Interfaces/wrong-answer-126515_Cue.wrong-answer-126515_Cue'");

    BlankCue = FString(L"/Script/Engine.SoundCue'"
                       L"/Game/_Game/Assets/Sounds/_Blank/blank-sound_Cue.blank-sound_Cue'");

    M_BeginOverlapSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*BeginOverlapSoundPath);

    BeginOverlapSound = M_BeginOverlapSoundPath->Object;

    M_EndOverlapSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*EndOverlapSoundPath);

    EndOverlapSound = M_EndOverlapSoundPath->Object;

    M_NullptrSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*NullptrSoundPath);

    NullptrSound = M_NullptrSoundPath->Object;

    M_BlankCue = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*BlankCue);

    CustomSound_1 = M_BlankCue->Object;
    CustomSound_2 = M_BlankCue->Object;
    CustomSound_3 = M_BlankCue->Object;
    CustomSound_4 = M_BlankCue->Object;
    CustomSound_5 = M_BlankCue->Object;
}

void ADebugSounds::BeginPlay() { Super::BeginPlay(); }

void ADebugSounds::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADebugSounds::PlayBeginOverlapSound() {

    //UGameplayStatics::PlaySound2D(GetWorld(), BeginOverlapSound);
}

void ADebugSounds::PlayEndOverlapSound() {

    //UGameplayStatics::PlaySound2D(GetWorld(), EndOverlapSound);
}

void ADebugSounds::PlayNullptrSound() {

    if (NullptrSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), NullptrSound);
    } else {
        ExitPrintErr("NullptrSound is nullptr");
        PlayNullptrSound();
    }
}

void ADebugSounds::PlayCustomSound_1() {

    if (CustomSound_1) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_1);
    } else {
        PrintLogErr("CustomSound_1 is nullptr");
    }
}

void ADebugSounds::PlayCustomSound_2() {

    if (CustomSound_2) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_2);
    } else {
        ExitPrintErr("CustomSound_2 is nullptr");
        PlayNullptrSound();
    }
}

void ADebugSounds::PlayCustomSound_3() {
    if (CustomSound_3) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_3);
    } else {
        ExitPrintErr("CustomSound_3 is nullptr");
        PlayNullptrSound();
    }
}

void ADebugSounds::PlayCustomSound_4() {
    if (CustomSound_4) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_4);
    } else {
        ExitPrintErr("CustomSound_4 is nullptr");
        PlayNullptrSound();
    }
}

void ADebugSounds::PlayCustomSound_5() {
    if (CustomSound_5) {
        UGameplayStatics::PlaySound2D(GetWorld(), CustomSound_5);
    } else {
        ExitPrintErr("CustomSound_5 is nullptr");
        PlayNullptrSound();
    }
}
