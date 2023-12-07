// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <iostream>
#include <limits>
#include <random>

#include "DefaultRandom.generated.h"

UCLASS()
class SHOOTER_API ADefaultRandom : public AActor {

    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADefaultRandom();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    std::mt19937                         RandomEngine32;
    std::mt19937_64                      RandomEngine64;
    std::uniform_int_distribution<int32> RandomInt32Distribution;
    std::uniform_int_distribution<int64> RandomInt64Distribution;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE int32 GenerateRandomInt32();
    FORCEINLINE int64 GenerateRandomInt64();
};
