// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Custom.h"
#include "GameFramework/Actor.h"

#include "DebugSounds.generated.h"

using namespace UnrealBasic;

UCLASS()
class SHOOTER_API ADebugSounds : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADebugSounds();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sounds", 
        meta = (AllowPrivateAccess = "true"))
    class USoundCue *BeginOverlapSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *EndOverlapSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *NullptrSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
