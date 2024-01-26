#include "NewTriggerBox.h"
#include "Custom.h"

#include <DrawDebugHelpers.h>

ANewTriggerBox::ANewTriggerBox() : TimerHandle(FTimerHandle())
, CoolDown(1.f)
, bCanOverlap(true){

    OnActorBeginOverlap.AddDynamic(this, &ANewTriggerBox::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ANewTriggerBox::OnOverlapEnd);
}

void ANewTriggerBox::BeginPlay() {

    Super::BeginPlay();

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 5);
}

void ANewTriggerBox::OnOverlapBegin(AActor *OverlappedActor, AActor *OtherActor) {

    CheckPtr(OtherActor);
    if (OtherActor == this || !bCanOverlap) return;

    bCanOverlap = false;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ANewTriggerBox::CoolDownOver, CoolDown);
    BeginOverlap(OverlappedActor, OtherActor);  
    
}

void ANewTriggerBox::BeginOverlap(AActor *OverlappedActor, AActor *OtherActor) {

    CLS();
    PrintOnScr("Overlap Begin");
    PrintOnScrFS("Overlapped Actor = %s", *OverlappedActor->GetName());
    PrintOnScrFS("OtherActor Actor = %s", *OtherActor->GetName());

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Red, true, -1, 0, 5);
}

void ANewTriggerBox::OnOverlapEnd(AActor *OverlappedActor, AActor *OtherActor) {

    CheckPtr(OtherActor);
    if (OtherActor == this || !bCanOverlap) return;

    bCanOverlap = false;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ANewTriggerBox::CoolDownOver, CoolDown);
    EndOverlap(OverlappedActor, OtherActor);   
}

void ANewTriggerBox::EndOverlap(AActor *OverlappedActor, AActor *OtherActor) {

    CLS();
    PrintOnScr("Overlap End");
    PrintOnScrFS("Overlapped Actor = %s", *OverlappedActor->GetName());
    PrintOnScrFS("OtherActor Actor = %s", *OtherActor->GetName());

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 5);
}


