#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8 {

    EWT_SubmachineGun UMETA(DisplayName = "Sub-machine Gun"),
    EWT_AssaultRifle  UMETA(DisplayName = "Assault Rifle"),

    EWT_MAX UMETA(DisplayName = "DefaultMAX")
};