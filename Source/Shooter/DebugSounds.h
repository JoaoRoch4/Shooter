#pragma once

#include "CoreMinimal.h"
#include "Custom.h"
#include "GameFramework/Actor.h"

#include "DebugSounds.generated.h"

UCLASS()
class SHOOTER_API ADebugSounds : public AActor {
    GENERATED_BODY()

public:

    // Sets default values for this actor's properties
    ADebugSounds();

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void SetSounds();

private:

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    class USoundCue *BeginOverlapSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *EndOverlapSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    USoundCue *NullptrSound;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
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

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    FString BeginOverlapSoundPath;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    FString EndOverlapSoundPath;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
    FString NullptrSoundPath;

    UPROPERTY(
      EditAnywhere, BlueprintReadWrite, Category = "Sounds", meta = (AllowPrivateAccess = "true")) 
    FString BlankCue;

    class TUniquePtr<struct ConstructorHelpers::FObjectFinder<class USoundCue>>
      M_BeginOverlapSoundPath;

    TUniquePtr<ConstructorHelpers::FObjectFinder<USoundCue>> M_EndOverlapSoundPath;

    TUniquePtr<ConstructorHelpers::FObjectFinder<USoundCue>> M_NullptrSoundPath;

    TUniquePtr<ConstructorHelpers::FObjectFinder<USoundCue>> M_BlankCue;

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
