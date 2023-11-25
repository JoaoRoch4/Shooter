#include "MyTriggerBox.h"

#include "DebugSounds.h"
#include "ShooterCharacter.h"

#include <iostream>
#include <limits>
#include <random>

#include <Components/BoxComponent.h>
#include <Components/ShapeComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

AMyTriggerBox::AMyTriggerBox()
 : DebugSounds(nullptr)
 , TimerHandle(FTimerHandle())
 , CoolDown(0.7f)
 , bCanOverlap(true)
 , bExitGame(false) {

    PrimaryActorTick.bCanEverTick = true;

    DebugSounds      = CDSubObj<ADebugSounds>(L"DebugSounds");
    Custom           = CDSubObj<ACustom>(L"Custom");

    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::BeginPlay() {

    Super::BeginPlay();


    DebugSounds = Cast<ADebugSounds>(
      UGameplayStatics::GetActorOfClass(GetWorld(), ADebugSounds::StaticClass()));

    //Custom = Cast<ACustom>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustom::StaticClass()));

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);
}

void AMyTriggerBox::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AMyTriggerBox::OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor) {

    if (OtherActor && (OtherActor != this)) {

        if (bCanOverlap) {

            bCanOverlap = false;

            if (bExitGame) ExitGame();
            OverlapBeginHappened(OverlappedActor, OtherActor);

            GetWorldTimerManager().SetTimer(
              TimerHandle, this, &AMyTriggerBox::CoolDownOver, CoolDown);
        }
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OnOverlapEnd(AActor *OverlappedActor, AActor *OtherActor) {

    if (OtherActor && (OtherActor != this)) {

        if (bCanOverlap) {

            bCanOverlap = false;
            OverlapEndHappened(OverlappedActor, OtherActor);

            GetWorldTimerManager().SetTimer(
              TimerHandle, this, &AMyTriggerBox::CoolDownOver, CoolDown);

        }
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OverlapBeginHappened(AActor *OverlappedActor, AActor *OtherActor) {

    PrintOnScr("OverlapBeginHappened");
    PrintOnScrFS("By: %s", *OtherActor->GetName());

    if (DebugSounds) {
        DebugSounds->PlayBeginOverlapSound();
        DebugSounds->PlayCustomSound_1();
    }

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Red, true, -1, 0, 10.f);

    int32 RandomInt {Custom->GenerateRandomInt32()};

    PrintOnScrFS("RandomInt: %s", *FString::FromInt(RandomInt));
}

void AMyTriggerBox::OverlapEndHappened(AActor *OverlappedActor, AActor *OtherActor) {

    PrintOnScr("OverlapEndHappened");
    PrintOnScrFS("By: %s", *OtherActor->GetName());

    if (DebugSounds) DebugSounds->PlayEndOverlapSound();
    
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);

    int64 RandomInt64 {Custom->GenerateRandomInt64()};

    PrintOnScrFS("RandomInt: %s", *FString::FromInt(RandomInt64));

    //ExitEngine();
}

void AMyTriggerBox::CoolDownOver() { bCanOverlap = true; }
