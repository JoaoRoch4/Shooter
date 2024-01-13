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

    EMD_Jump UMETA(DisplayName = "Jump"),

    EMD_Aim UMETA(DisplayName = "Aim"),
    EMD_ForwardAim UMETA(DisplayName = "ForwardAim"),
    EMD_BackwardAim UMETA(DisplayName = "BackwardAim"),
    EMD_RightAim UMETA(DisplayName = "RightAim"),
    EMD_LeftAim UMETA(DisplayName = "LeftAim"),
    EMD_ForwardRightAim UMETA(DisplayName = "ForwardRightAim"),
    EMD_ForwardLeftAim UMETA(DisplayName = "ForwardLeftAim"),
    EMD_BackwardRightAim UMETA(DisplayName = "BackwardRightAim"),
    EMD_BackwardLeftAim UMETA(DisplayName = "BackwardLeftAim"),

    EMD_Crouch UMETA(DisplayName = "Crouch"),
    EMD_ForwardCrouch UMETA(DisplayName = "ForwardCrouch"),
    EMD_BackwardCrouch UMETA(DisplayName = "BackwardCrouch"),
    EMD_RightCrouch UMETA(DisplayName = "RightCrouch"),
    EMD_LeftCrouch UMETA(DisplayName = "LeftCrouch"),
    EMD_ForwardRightCrouch UMETA(DisplayName = "ForwardRightCrouch"),
    EMD_ForwardLeftCrouch UMETA(DisplayName = "ForwardLeftCrouch"),
    EMD_BackwardRightCrouch UMETA(DisplayName = "BackwardRightCrouch"),
    EMD_BackwardLeftCrouch UMETA(DisplayName = "BackwardLeftCrouch"),

    EMD_CrouchAim UMETA(DisplayName = "CrouchAim"),
    EMD_ForwardCrouchAim UMETA(DisplayName = "ForwardCrouchAim"),
    EMD_BackwardCrouchAim UMETA(DisplayName = "BackwardCrouchAim"),
    EMD_RightCrouchAim UMETA(DisplayName = "RightCrouchAim"),
    EMD_LeftCrouchAim UMETA(DisplayName = "LeftCrouchAim"),
    EMD_ForwardRightCrouchAim UMETA(DisplayName = "ForwardRightCrouchAim"),
    EMD_ForwardLeftCrouchAim UMETA(DisplayName = "ForwardLeftCrouchAim"),
    EMD_BackwardRightCrouchAim UMETA(DisplayName = "BackwardRightCrouchAim"),
    EMD_BackwardLeftCrouchAim UMETA(DisplayName = "BackwardLeftCrouchAim"),

    EMD_None UMETA(DisplayName = "None"),

    EMD_MAX UMETA(DisplayName = "Default MAX")

};