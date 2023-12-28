#pragma once

UENUM(BlueprintType)
enum class EMovingDirection : uint8 {

    EMD_Forward UMETA(DisplayName = "Forward"),

    EMD_Backward UMETA(DisplayName = "Backward"),

    EMD_Right UMETA(DisplayName = "Right"),

    EMD_Left UMETA(DisplayName = "Left"),

    EMD_ForwardRight UMETA(DisplayName = "ForwardRight"),

    EMD_ForwardLeft UMETA(DisplayName = "ForwardLeft"),

    EMD_BackwardRight UMETA(DisplayName = "BackwardRight"),

    EMD_BackwardLeft UMETA(DisplayName = "BackwardLeft"),

    EMD_Aim UMETA(DisplayName = "Aim"),

    EMD_ForwardAim UMETA(DisplayName = "ForwardAim"),

    EMD_BackwardAim UMETA(DisplayName = "BackwardAim"),

    EMD_RightAim UMETA(DisplayName = "RightAim"),

    EMD_LeftAim UMETA(DisplayName = "LeftAim"),

    EMD_ForwardRightAim UMETA(DisplayName = "ForwardRightAim"),

    EMD_ForwardLeftAim UMETA(DisplayName = "ForwardLeftAim"),

    EMD_BackwardRightAim UMETA(DisplayName = "BackwardRightAim"),

    EMD_BackwardLeftAim UMETA(DisplayName = "BackwardLeftAim"),

    EMD_None UMETA(DisplayName = "None"),

    EMD_MAX UMETA(DisplayName = "Default MAX")

};