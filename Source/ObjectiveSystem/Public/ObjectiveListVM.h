// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "ObjectiveDeactivationReason.h"
#include "Components/SlateWrapperTypes.h"

#include "ObjectiveListVM.generated.h"

/**
 * View model for a list of active objectives.
 */
UCLASS()
class OBJECTIVESYSTEM_API UObjectiveListVM : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// Bind this VM to objective (de)activation events on the objective service.
	UFUNCTION(BlueprintCallable)
	void Initialize();

	// VMs of minor objectives.
	UPROPERTY(FieldNotify, BlueprintReadOnly)
	TArray<TObjectPtr<UObject>> MinorObjectivesVMs;
	
	// VMs of major objectives.
	UPROPERTY(FieldNotify, BlueprintReadOnly)
	TArray<TObjectPtr<UObject>> MajorObjectivesVMs;

	// Visibility of the associated objectives widget container.
	UPROPERTY(FieldNotify, BlueprintReadOnly)
	ESlateVisibility Visibility = ESlateVisibility::Hidden;

protected:
	/**
	 * When an objective was activated, set the objectives widget container to visible,
	 * create a @UObjectiveVM, initialize it, and add it to Minor/MajorObjectiveVMs.
	 * Finally trigger the respective field notifiers.
	 * 
	 * @param ObjectiveID - the objective that is being activated
	 */
	void HandleOnObjectiveActivated(FGameplayTag ObjectiveID);

	/**
	 * When an objective was deactivated, request a deactivation animation from the
	 * objectives VM and bind to its animation finished delegate.
	 * 
	 * @param ObjectiveID - the objective that is being deactivated
	 * @param DeactivationReason - the reason for deactivation, influencing the visuals
	 */
	void HandleOnObjectiveDeactivated(FGameplayTag ObjectiveID, EObjectiveDeactivationReason DeactivationReason);

	/**
	 * When the deactivation animation is finished, remove the objectives VM from internal
	 * bookkeeping and potentially set the objectives widget container hidden.
	 * 
	 * @param ObjectiveID - the objective that is being deactivated
	 * @param bMinor - whether the objective is minor
	 */
	void HandleOnEntryDeactivationAnimationFinished(FGameplayTag ObjectiveID, bool bMinor);

	// Keep indices to where in @MinorObjectivesVMs which objective ID has its VM.
	TMap<FGameplayTag, int> MinorObjectivesVMIndices;
	// Keep indices to where in @MajorObjectivesVMs which objective ID has its VM.
	TMap<FGameplayTag, int> MajorObjectivesVMIndices;
};
