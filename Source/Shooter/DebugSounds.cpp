#include "DebugSounds.h"

#include "Templates/UniquePtr.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundCue.h>

ADebugSounds::ADebugSounds()
 : BeginOverlapSound(nullptr)
 , EndOverlapSound(nullptr)
 , NullptrSound(nullptr)
 , CustomSound_1(nullptr)
 , CustomSound_2(nullptr)
 , CustomSound_3(nullptr)
 , CustomSound_4(nullptr)
 , CustomSound_5(nullptr) 
 , M_BeginOverlapSoundPath(nullptr)
 , M_EndOverlapSoundPath(nullptr) 
 , M_NullptrSoundPath(nullptr)
 , M_BlankCue(nullptr)
{
    PrimaryActorTick.bCanEverTick = true;

    BeginOverlapSound = CreateDefaultSubobject<USoundCue>(L"BeginOverlapSound");
    EndOverlapSound   = CreateDefaultSubobject<USoundCue>(L"EndOverlapSound");
    NullptrSound      = CreateDefaultSubobject<USoundCue>(L"NullptrSound");
    CustomSound_1     = CreateDefaultSubobject<USoundCue>(L"CustomSound_1");
    CustomSound_2     = CreateDefaultSubobject<USoundCue>(L"CustomSound_2");
    CustomSound_3     = CreateDefaultSubobject<USoundCue>(L"CustomSound_3");
    CustomSound_4     = CreateDefaultSubobject<USoundCue>(L"CustomSound_4");
    CustomSound_5     = CreateDefaultSubobject<USoundCue>(L"CustomSound_5");

    SetSounds();
}

void ADebugSounds::SetSounds() {

     const static FString BeginOverlapSoundPath {
      L"/Script/Engine.SoundCue'"
      L"/Game/_Game/Assets/Sounds/Interfaces/button-124476_Cue.button-124476_Cue'"};

    const static FString EndOverlapSoundPath {
      L"/Script/Engine.SoundCue'"
      L"/Game/_Game/Assets/Sounds/Interfaces/interface-124464_Cue.interface-124464_Cue'"};

    const static FString NullptrSoundPath {
      L"/Script/Engine.SoundCue'"
      L"/Game/_Game/Assets/Sounds/Interfaces/wrong-answer-126515_Cue.wrong-answer-126515_Cue'"};

    const static FString BlankCue {
      L"/Script/Engine.SoundCue'"
      L"/Game/_Game/Assets/Sounds/_Blank/blank-sound_Cue.blank-sound_Cue'"};

    M_BeginOverlapSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*BeginOverlapSoundPath);

    if (M_BeginOverlapSoundPath->Succeeded()) {
        BeginOverlapSound = M_BeginOverlapSoundPath->Object;
    } else {
        ExitGameErr("M_BeginOverlapSoundPath failed");
    }

    M_EndOverlapSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*EndOverlapSoundPath);

    if (M_EndOverlapSoundPath->Succeeded()) {
        EndOverlapSound = M_EndOverlapSoundPath->Object;
    } else {
        ExitGameErr("M_EndOverlapSoundPath failed");
    }

    M_NullptrSoundPath
      = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*NullptrSoundPath);

    if (M_NullptrSoundPath->Succeeded()) {
        NullptrSound = M_NullptrSoundPath->Object;
    } else {
        ExitGameErr("M_NullptrSoundPath failed");
    }

    M_BlankCue = MakeUnique<ConstructorHelpers::FObjectFinder<USoundCue>>(*BlankCue);

    if (M_BlankCue->Succeeded()) {

        CustomSound_1 = M_BlankCue->Object;
        CustomSound_2 = M_BlankCue->Object;
        CustomSound_3 = M_BlankCue->Object;
        CustomSound_4 = M_BlankCue->Object;
        CustomSound_5 = M_BlankCue->Object;

    } else {
        ExitGameErr("M_BlankCue failed");
    }
}

void ADebugSounds::BeginPlay() { Super::BeginPlay(); }

void ADebugSounds::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADebugSounds::PlayBeginOverlapSound() {

    if (BeginOverlapSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), BeginOverlapSound);
    } else {
        ExitPrintErr("BeginOverlapSound is nullptr");
        PlayNullptrSound();
    }
}

void ADebugSounds::PlayEndOverlapSound() {

    if (EndOverlapSound) {
        UGameplayStatics::PlaySound2D(GetWorld(), EndOverlapSound);
    } else {
        ExitPrintErr("EndOverlapSound is nullptr");
        PlayNullptrSound();
    }
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
