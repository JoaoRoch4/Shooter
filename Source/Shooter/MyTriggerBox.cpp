#include "MyTriggerBox.h"

#include "DebugSounds.h"
#include "DefaultRandom.h"
#include "ShooterCharacter.h"

#include <iostream>
#include <limits>
#include <random>

#include <Components/BoxComponent.h>
#include <Components/ShapeComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Sound/SoundCue.h>

AMyTriggerBox::AMyTriggerBox()
 : DebugSounds(nullptr)
 , TimerHandle(FTimerHandle())
 , CoolDown(0.7f)
 , bCanOverlap(true)
 , bExitGame(false)
 , Custom(nullptr)
 , DefaultRandom(nullptr) {

    PrimaryActorTick.bCanEverTick = true;

    Custom            = CDSubObj<ACustom>(L"Custom");
    DefaultRandom     = CDSubObj<ADefaultRandom>(L"DefaultRandom");
    

   OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
   OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::BeginPlay() {

    Super::BeginPlay();

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
            //OverlapEndHappened(OverlappedActor, OtherActor);

            GetWorldTimerManager().SetTimer(
              TimerHandle, this, &AMyTriggerBox::CoolDownOver, CoolDown);
        }
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OverlapBeginHappened(AActor *OverlappedActor, AActor *OtherActor) {

    PrintClearScreen();
    PrintOnScr("OverlapBeginHappened");
    PrintOnScrFS("By: %s", *OtherActor->GetName());
    
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Red, true, -1, 0, 10.f);

    int32 RandomInt {DefaultRandom->GenerateRandomInt32()};   
        PrintOnScrFS("RandomInt: %s", *FString::FromInt(RandomInt));
}

void AMyTriggerBox::OverlapEndHappened(AActor *OverlappedActor, AActor *OtherActor) {

    PrintClearScreen();

    PrintOnScr("OverlapEndHappened");
    PrintOnScrFS("By: %s", *OtherActor->GetName());

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);

    int64 RandomInt64 {DefaultRandom->GenerateRandomInt64()};

    PrintOnScrFS("RandomInt: %s", *FString::FromInt(RandomInt64));

   
    // ExitEngine();
}

void AMyTriggerBox::CoolDownOver() { bCanOverlap = true; }
