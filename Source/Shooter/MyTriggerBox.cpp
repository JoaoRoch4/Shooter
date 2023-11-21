#include "MyTriggerBox.h"

#include "DebugSounds.h"
#include "ShooterCharacter.h"

#include <iostream>

#include <Components/BoxComponent.h>
#include <Components/ShapeComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

AMyTriggerBox::AMyTriggerBox()
 : ShooterCharacter(nullptr)
 , DebugSounds(nullptr)
 , TimerHandle(FTimerHandle())
 , CoolDown(0.7f)
 , bCanOverlap(true)
 , bExitGame(false) {

    PrimaryActorTick.bCanEverTick = true;

    ShooterCharacter = CDSubObj<AShooterCharacter>(L"ShooterCharacter");
    DebugSounds      = CDSubObj<ADebugSounds>(L"DebugSounds");

    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::BeginPlay() {

    Super::BeginPlay();

    ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    DebugSounds = Cast<ADebugSounds>(
      UGameplayStatics::GetActorOfClass(GetWorld(), ADebugSounds::StaticClass()));

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);
}

void AMyTriggerBox::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AMyTriggerBox::OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor) {

    if (OtherActor && (OtherActor != this)) {

        if (bCanOverlap) {

            bCanOverlap = false;

            if (bExitGame) ExitGame();
            OverlapBeginHappened();

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
            OverlapEndHappened();

            GetWorldTimerManager().SetTimer(
              TimerHandle, this, &AMyTriggerBox::CoolDownOver, CoolDown);
        }
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OverlapBeginHappened() {

    PrintOnScr("OverlapBeginHappened");
    PrintOnScrFS("By: %s", *ShooterCharacter->GetName());

    if (DebugSounds) {
        DebugSounds->PlayBeginOverlapSound();
        DebugSounds->PlayCustomSound_1();
    }

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Red, true, -1, 0, 10.f);
}

void AMyTriggerBox::OverlapEndHappened() {

    PrintOnScr("OverlapEndHappened");
    PrintOnScrFS("By: %s", *ShooterCharacter->GetName());

    if (DebugSounds) {
        DebugSounds->PlayEndOverlapSound();
        DebugSounds->PlayNullptrSound();
    }

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);
}

void AMyTriggerBox::CoolDownOver() { bCanOverlap = true; }
