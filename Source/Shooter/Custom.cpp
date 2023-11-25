// Fill out your copyright notice in the Description page of Project Settings.

#include "Custom.h"


// Sets default values
ACustom::ACustom()
 : RandomEngine32(std::random_device {}())
 , RandomEngine64(std::random_device {}())
 , RandomInt32Distribution(std::numeric_limits<int32>::min(), std::numeric_limits<int32>::max())
 , RandomInt64Distribution(std::numeric_limits<int64>::min(), std::numeric_limits<int64>::max()) {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

void ACustom::BeginPlay() { Super::BeginPlay(); }

void ACustom::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

int32 ACustom::GenerateRandomInt32() { return RandomInt32Distribution(RandomEngine32); }

int64 ACustom::GenerateRandomInt64() { return RandomInt64Distribution(RandomEngine64); }

FString ACustom::IntToString(const int32 Int) {
    FString IntString {FString::FromInt(Int)};
    return IntString;
}

FString ACustom::RealToString(const float Float) {
    FString FloatString {FString::SanitizeFloat(Float)};
    return FloatString;
}

FString ACustom::DoubleToString(const double Double) {
    FString DoubleString {FString::SanitizeFloat(Double)};
    return DoubleString;
}

float ACustom::DoubleToFloat(const double Double) { return StaticCast<const float>(Double); }
