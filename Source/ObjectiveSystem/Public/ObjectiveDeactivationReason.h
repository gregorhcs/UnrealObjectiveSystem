// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Reasons why an objective is deactivated.
 */
UENUM(BlueprintType)
enum class EObjectiveDeactivationReason : uint8
{
	Succeeded,
	LostRelevance,
	Failed
};
