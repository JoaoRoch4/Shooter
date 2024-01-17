// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultRandom.h"

// Sets default values
ADefaultRandom::ADefaultRandom()
 : RandomEngine32(std::random_device {}())
 , RandomEngine64(std::random_device {}())
 , RandomInt32Distribution(std::numeric_limits<int32>::min(), std::numeric_limits<int32>::max())
 , RandomInt64Distribution(std::numeric_limits<int64>::min(), std::numeric_limits<int64>::max()) {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if
    // you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADefaultRandom::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADefaultRandom::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

int32 ADefaultRandom::GenerateRandomInt32() { return RandomInt32Distribution(RandomEngine32); }

int64 ADefaultRandom::GenerateRandomInt64() { return RandomInt64Distribution(RandomEngine64); }
