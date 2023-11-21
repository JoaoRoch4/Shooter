#include "MyTriggerBox.h"
#include "ShooterCharacter.h"

#include <iostream>

#include <Components/BoxComponent.h>
#include <Components/ShapeComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>

AMyTriggerBox::AMyTriggerBox() : ShooterCharacter(nullptr) {

    PrimaryActorTick.bCanEverTick = true;
        
    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapEnd);
}

void AMyTriggerBox::BeginPlay() {

    Super::BeginPlay();

    ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);
}

void AMyTriggerBox::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AMyTriggerBox::OnOverlapBegin(class AActor *OverlappedActor, class AActor *OtherActor) {

    if (OtherActor && (OtherActor != this)) {
        return OverlapBeginHappened();
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OnOverlapEnd(AActor *OverlappedActor, AActor *OtherActor) {

    if (OtherActor && (OtherActor != this)) {
        return OverlapEndHappened();
    } else {
        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::OverlapBeginHappened() {

    PrintOnScr("OverlapBeginHappened");
    PrintOnScrFS("By: %s", *ShooterCharacter->GetName());
}

void AMyTriggerBox::OverlapEndHappened() {

    PrintOnScr("OverlapEndHappened");
    PrintOnScrFS("By: %s", *ShooterCharacter->GetName());

}
