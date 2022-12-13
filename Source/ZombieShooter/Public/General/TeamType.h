

#pragma once

#include "CoreMinimal.h"

/**
 * Based on the ETeamAttitude from GenericTeamAgentInterface, but can be expanded for more teams.
 */
UENUM(BlueprintType)
enum class ETeamType : uint8 {
    NEUTRAL = 0 UMETA(DisplayName = "NEUTRAL"),
    ZOMBIE = 1  UMETA(DisplayName = "ZOMBIE"),
    PLAYER = 2     UMETA(DisplayName = "PLAYER")
};
