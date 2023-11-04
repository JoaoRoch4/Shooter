// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom.h"

#include <iostream>
#include <string>
#include <Any>

// Sets default values
ACustom::ACustom() {
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustom::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ACustom::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

inline FString ACustom::IntToString(const int32 Int) {
    FString IntString {FString::FromInt(Int)};
    return IntString;
}

inline FString ACustom::RealToString(const float Float) {
    FString FloatString {FString::SanitizeFloat(Float)};
    return FloatString;
}

inline FString ACustom::DoubleToString(const double Double) {
    FString DoubleString {FString::SanitizeFloat(Double)};
    return DoubleString;
}

inline float ACustom::DoubleToFloat(const double Double) {
    return StaticCast<const float>(Double);
}
