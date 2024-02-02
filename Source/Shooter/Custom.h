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

#define PrintOnScr(Text)                                                                           \
    if (GEngine) {                                                                                 \
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Text);                              \
    }
#define PrintOnScrColor(Color, Text)                                                               \
    if (GEngine) {                                                                                 \
        GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, Text);                                    \
    }
#define PrintOnScrColorTime(Time, Color, Text)                                                     \
    if (GEngine) {                                                                                 \
        GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);                                   \
    }

#define PrintOnScrTime(Time, Text)                                                                 \
    if (GEngine) {                                                                                 \
        GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, Text);                             \
    }

#define PrintOnScrColorTimeW_Else(Time, Color, Text)                                               \
    PrintOnScrColorTime(Time, Color, Text) GEngineNullptrMessage

#define PrintOnScrFS(text, fstring)                                                                \
    if (GEngine) {                                                                                 \
        GEngine->AddOnScreenDebugMessage(                                                          \
          -1, 5.f, FColor::Red, FString::Printf(TEXT(text), fstring));                             \
    }

#define GEngineNullptrMessage                                                                      \
    else {                                                                                         \
        PL_Err("GEngine was Nullptr");                                                             \
    }

#define PrintLog_and_scr(Text) PrintLog(Text) PrintOnScr(Text) GEngineNullptrMessage

#define OverlapArgumentsBegin                                                                      \
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,  \
      int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult

#define OverlapArgumentsEnd                                                                        \
    UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,  \
      int32 OtherBodyIndex

#define CDSO CreateDefaultSubobject

#define CDSubObj CDSO

typedef FVector   Fvc;
typedef FVector2d Fvc2;

#define ZeroFVec FVector(0.f)

#define CLS()                                                                                      \
    if (GEngine) GEngine->ClearOnScreenDebugMessages()
#define PrintCls()         CLS()
#define PrintClearScreen() CLS()

#define RR(X, Y) FMath::RandRange(X, Y)

#define PrintLogFVec(FVec)                                                                         \
    PrintLog(FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), FVec.X, FVec.Y, FVec.Z))

#define ConsoleWrt(X)                                                                              \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController())              \
    PlayerController->ConsoleCommand(TEXT(X))

#include <Kismet/KismetSystemLibrary.h>
#define ExitGame()                                                                                 \
    UKismetSystemLibrary::QuitGame(                                                                \
      GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);

#define ExitGameErr(X)                                                                             \
    if (APlayerController *PlayerController = GetWorld()->GetFirstPlayerController()) {            \
        checkf(false, TEXT(X));                                                                    \
    }

#define ExitPrintErr(X) PrintLogErr(X) ExitGame()

#define QuitGamePrintErr(X) ExitPrintErr(X)

#define __localEnsureCheckPtr(ptr)                                                                 \
    const FString Msg {FString::Printf(                                                            \
      TEXT("%s is nullptr: File: %s, Line: %d"), TEXT(#ptr), TEXT(__FILE__), __LINE__)};           \
    if (!(ensureMsgf((ptr) != nullptr, TEXT("%s"), *Msg))) {                                       \
        UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);                                                  \
        return ExitGame();                                                                         \
    }

#define __localCheckF_CheckPtr(ptr)                                                                \
    const FString Msg {FString::Printf(                                                            \
      TEXT("%s is nullptr: File: %s, Line: %d"), TEXT(#ptr), TEXT(__FILE__), __LINE__)};           \
    checkf((ptr) != nullptr, TEXT("%s"), *Msg);

#define CheckPtr(ptr)                                                                              \
    if (ptr == nullptr) {                                                                          \
        __localEnsureCheckPtr(ptr);                                                                \
    }

#define CheckMsgPtr(ptr)                                                                           \
    if (ptr == nullptr) {                                                                          \
        __localEnsureCheckPtr(ptr)                                                                 \
    } else {                                                                                       \
        UE_LOG(LogTemp, Log, TEXT("%s is valid"), TEXT(#ptr));                                     \
    }

#define CheckPtrLegacy(ptr, InFormat, ...)                                                         \
    if (!(ensureMsgf((ptr) != nullptr, TEXT(InFormat), ##__VA_ARGS__))) {                          \
        PrintLogErr(InFormat);                                                                     \
        ExitGame()                                                                                 \
    }

#define CheckExpression(InExpression, InFormat, ...)                                               \
    if (!(ensureMsgf(InExpression, TEXT(InFormat), ##__VA_ARGS__))) {                              \
        PrintLogErr(InFormat);                                                                     \
        ExitGame()                                                                                 \
    }

#define CheckPtrExit(ptr) __localCheckF_CheckPtr(ptr)


#include <GenericPlatform/GenericPlatformApplicationMisc.h>
#define ExitEngine() FGenericPlatformMisc::RequestExit(false)

// #define QuitGame() ExitGame()

#define ClearLog()                                                                                 \
    if (GEngine) GEngine->Exec(GetWorld(), TEXT("Clear"));

#define self this


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Kismet/KismetSystemLibrary.h>
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
    FString IntToString(const int32 Int);

    /**
     * @brief Get a real number and convert it to FString
     * @param
     * Float To be converted
     * @return FString of the value of the float
     */
    UFUNCTION(BlueprintCallable, Category = "Conversion")
    FString RealToString(const float Float);

    UFUNCTION(BlueprintCallable, Category = "Conversion")
    FString DoubleToString(const double Double);

    UFUNCTION(BlueprintCallable, Category = "Conversion")
    float DoubleToFloat(const double Double);
};