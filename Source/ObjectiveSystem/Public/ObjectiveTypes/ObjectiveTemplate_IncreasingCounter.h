// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveTemplate.h"
#include "ObjectiveTemplate_IncreasingCounter.generated.h"

/**
 * Template for an objective with increasing, capped counter.
 */
UCLASS(DisplayName="Objective With Increasing Counter")
class OBJECTIVESYSTEM_API UObjectiveTemplate_IncreasingCounter : public UObjectiveTemplate
{
	GENERATED_BODY()

public:
	UObjectiveTemplate_IncreasingCounter();

	// Get the counter cap (inclusive).
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCounterMax() { return CounterMax; }

protected:
	// The counters cap (inclusive).
	UPROPERTY(EditDefaultsOnly)
	int CounterMax = 5;
};
