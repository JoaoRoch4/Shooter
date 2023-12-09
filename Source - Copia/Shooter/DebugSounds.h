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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sounds", 
        meta = (AllowPrivateAccess = "true"))
    class USoundCue *BeginOverlapSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *EndOverlapSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *NullptrSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds",
      meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound_1;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound_2;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound_3;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound_4;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *CustomSound_5;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // crate a play sound function for each sound
    UFUNCTION(BlueprintCallable)
    void PlayBeginOverlapSound();

    UFUNCTION(BlueprintCallable)
    void PlayEndOverlapSound();

    UFUNCTION(BlueprintCallable)
    void PlayNullptrSound();

    UFUNCTION(BlueprintCallable)
    void PlayCustomSound_1();

    UFUNCTION(BlueprintCallable)
    void PlayCustomSound_2();

    UFUNCTION(BlueprintCallable)
    void PlayCustomSound_3();

    UFUNCTION(BlueprintCallable)
    void PlayCustomSound_4();

    UFUNCTION(BlueprintCallable)
    void PlayCustomSound_5();
};