// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define elif else if

#define PrintLog(Text)        UE_LOG(LogTemp, Log, TEXT(Text))
#define PL_Log(Text)          PrintLog(Text)
#define PrintLogWarning(Text) UE_LOG(LogTemp, Warning, TEXT(Text))
#define PL_Warning(Text)      UE_LOG(LogTemp, Warning, TEXT(Text))
#define PrintLogErr(Text)     UE_LOG(LogTemp, Error, TEXT(Text))
#define PL_Err(Text)          UE_LOG(LogTemp, Error, TEXT(Text));
#define PrintLogBreakLine()   UE_LOG(LogTemp, Error, TEXT(" "));
#define PL_BreakLine()        UE_LOG(LogTemp, Error, TEXT(" "));
#define PrintOnScr(Text)                                                                                     \
    if (GEngine) {                                                                                           \
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Text);                                        \
    }
#define PrintOnScrColor(Color, Text)                                                                         \
    if (GEngine) {                                                                                           \
        GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Text);                                              \
    }
#define PrintOnScrColorTime(Time, Color, Text)                                                               \
    if (GEngine) {                                                                                           \
        GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);                                             \
    }

#define PrintOnScrTime(Time, Text)                                                                           \
    if (GEngine) {                                                                                           \
        GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, Text);                                       \
    }

#define PrintOnScrColorTimeW_Else(Time, Color, Text)                                                         \
    PrintOnScrColorTime(Time, Color, Text) GEngineNullptrMessage

#define PrintOnScrFS(text, fstring)                                                                          \
    if (GEngine) {                                                                                           \
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(text), fstring));        \
    }

#define GEngineNullptrMessage                                                                                \
    else {                                                                                                   \
        PL_Err("GEngine was Nullptr");                                                                       \
    }

#define PrintLog_and_scr(Text) PrintLog(Text) PrintOnScr(Text) GEngineNullptrMessage

#define OverlapArgumentsBegin                                                                                \
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,            \
      int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult

#define OverlapArgumentsEnd                                                                                  \
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,            \
      int32 OtherBodyIndex

#define MyMacros_BeginOverlapAddDynamic(func)                                                                \
    OnActorBeginOverlap.__Internal_AddDynamic(                                                               \
      this, func, UE4Delegates_Private::GetTrimmedMemberFunctionName((TEXT(#func))))
#define BeginOverlapAddDynamic(func) MyMacros_BeginOverlapAddDynamic(func)

#define MyMacros_EndOverlapAddDynamic(func)                                                                  \
    OnActorEndOverlap.__Internal_AddDynamic(                                                                 \
      this, func, UE4Delegates_Private::GetTrimmedMemberFunctionName((TEXT(#func))))

#define BeginOverlap(func) MyMacros_BeginOverlapAddDynamic(func)
#define EndOverlap(func)   MyMacros_EndOverlapAddDynamic(func)

#define MyMacros_BeginOverlapComponent(func)                                                                 \
    OnComponentBeginOverlap.__Internal_AddDynamic\(                                                          \
      this, func, UE4Delegates_Private::GetTrimmedMemberFunctionName((TEXT(#func))))

#define BeginOverlapComponent(func) MyMacros_BeginOverlapComponent(func)

#define CDSO CreateDefaultSubobject

#define CDSubObj CDSO

typedef FVector   Fvc;
typedef FVector2d Fvc2;

#define ZeroFVec FVector(0.f)


#define CLS()                                                                                                \
    if (GEngine) GEngine->ClearOnScreenDebugMessages()
#define PrintCls()         CLS()
#define PrintClearScreen() CLS()

#define RR(X, Y) FMath::RandRange(X, Y)

#define PrintLogFVec(FVec) PrintLog(FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), FVec.X, FVec.Y, FVec.Z))

#define ConsoleWrt(X)                                                                                        \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController())                        \
    PlayerController->ConsoleCommand(TEXT(X))

#define ExitGame(X)                                                                                          \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController())                        \
    PlayerController->ConsoleCommand(TEXT("Exit"))

#define ExitGameErr(X)                                                                                       \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController()) {                      \
        checkf(false, TEXT(X)); \
    }

#define ExitPrintErr(X)                                                                                      \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController()) {                      \
        PrintLogErr(X);                                                                                      \
        checkf(false, TEXT(X));                                                                              \
        PlayerController->ConsoleCommand(TEXT("Exit"));                                                      \
    }

#define ClearLog()                                                                                           \
    if (GEngine) GEngine->Exec(GetWorld(), TEXT("Clear"));                                                   
    

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Custom.generated.h"

/** Includes all the namespaces unreal have. */
namespace UnrealAll {

    using namespace UC;
    using namespace UF;
    using namespace UI;
    using namespace UM;
    using namespace UP;
}

/** Includes the namespaces UP, UF, and UM. */
namespace UnrealBasic {

    using namespace UP;
    using namespace UF;
    using namespace UM;
}

/**
 * 
 */
UCLASS()
class SHOOTER_API ACustom : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ACustom();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /**
    * @brief Get a int and convert it to FString
    * @param Int To be
     * converted
    * @return FString of the value of the Int
    */
    UFUNCTION(BlueprintCallable, Category = "Conversion")
    static FString IntToString(const int32 Int);

    /**
    * @brief Get a real number and convert it to FString
    * @param
     * Float To be converted
    * @return FString of the value of the float
 */
    UFUNCTION(BlueprintCallable, Category = "Conversion")
    static FString RealToString(const float Float);

    UFUNCTION(BlueprintCallable, Category = "Conversion")
    static FString DoubleToString(const double Double);

    UFUNCTION(BlueprintCallable, Category = "Conversion")
    static float DoubleToFloat(const double Double);
};
