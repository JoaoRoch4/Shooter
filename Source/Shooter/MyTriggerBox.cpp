#include "MyTriggerBox.h"
#include "ShooterCharacter.h"

#include <Components/BoxComponent.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>

AMyTriggerBox::AMyTriggerBox() : TriggerBox(nullptr), ShooterCharacter(nullptr) {

    PrimaryActorTick.bCanEverTick = true;


    //SetTriggerBoxProperties();

    //UStaticMeshComponent *VisualMesh      = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
}

void AMyTriggerBox::SetTriggerBoxProperties() {

    TriggerBox = CDSubObj<UBoxComponent>(L"TriggerBox");

    TriggerBox->SetWorldScale3D(Fvc(5.f));
    TriggerBox->SetBoxExtent(Fvc(10.f));
    TriggerBox->SetLineThickness(10.f);
    TriggerBox->SetCollisionProfileName(L"Trigger");
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootComponent = TriggerBox;

    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMyTriggerBox::OnOverlapBegin);
}

void AMyTriggerBox::BeginPlay() {

    Super::BeginPlay();

    ShooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
      FColor::Purple, true, -1, 0, 10.f);
}

void AMyTriggerBox::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);
}

void AMyTriggerBox::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
  const FHitResult &SweepResult) {

    if (OtherActor && (OtherActor != this)) {

        return BeginOverlapHapped();
    } else {

        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::BeginOverlapHapped() {

    PrintOnScr("Overlap Begin");
    PrintOnScrFS("Overlapped Actor = %s", *ShooterCharacter->GetName());
}

void AMyTriggerBox::OnOverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor,
  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {

    if (OtherActor && (OtherActor != this)) {

        return EndOverlapHapped();

    } else {

        ExitGameErr("OtherActor is nullptr");
    }
}

void AMyTriggerBox::EndOverlapHapped() {

    PrintOnScr("Overlap End");
    PrintOnScrFS("Overlapped Actor = %s", *ShooterCharacter->GetName());
}
