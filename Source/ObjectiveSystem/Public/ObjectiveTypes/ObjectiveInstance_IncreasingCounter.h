// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveInstance.h"
#include "ObjectiveTemplate_IncreasingCounter.h"
#include "GameService/GameServiceUser.h"
#include "ObjectiveInstance_IncreasingCounter.generated.h"

/**
 * Objective instance for an objective with a capped counter that can only increase.
 * Contains a counter API and will change its title based on the current counter that is internally managed.
 */
UCLASS()
class OBJECTIVESYSTEM_API UObjectiveInstance_IncreasingCounter
	: public UObjectiveInstance
	, public FGameServiceUser
{
	GENERATED_BODY()

public:
	UObjectiveInstance_IncreasingCounter();
	
	// - UObjectiveInstance
	virtual void Activate(UObjectiveTemplate* InOwningObjective) override;
	virtual FText GetCurrentTitle() override;
	// --

	// Increment the counter by one.
	UFUNCTION(BlueprintCallable)
	void IncrementCounter(bool& OutIsAtMax, int& OutNewCount);

	// Increase the counter by @Count.
	UFUNCTION(BlueprintCallable)
	void IncreaseCounter(int Amount, bool& OutIsAtMax, int& OutNewCount);

	// Get the current counter value.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentCounter() const;

	// Whether the current counter value is zero.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCounterAtZero() const;

	// Whether the current counter value is at the cap determined in the @CounterObjectiveTemplate.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCounterAtMax() const;

protected:
	// The current counter value.
	UPROPERTY(Transient)
	int CurrentCounter = 0;
	
	// This objectives template, down-casted.
	UPROPERTY(BlueprintReadOnly, Transient)
	UObjectiveTemplate_IncreasingCounter* CounterObjectiveTemplate = nullptr;
};
