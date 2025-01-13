// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "ObjectiveVM.generated.h"

class UObjectiveInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeactivationAnimationRequested, EObjectiveDeactivationReason, DeactivationReason);
DECLARE_MULTICAST_DELEGATE(FOnDeactivationAnimationFinished);

/**
 * View model for a single objective instance.
 */
UCLASS(Blueprintable)
class OBJECTIVESYSTEM_API UObjectiveVM : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Inject the objective instance this view model is representing. */
	void Initialize(UObjectiveInstance* InObjective);

	/**
	 * Broadcast @OnDeactivationAnimationFinished to let users know this VM no
	 * longer requires the objective instance.
	 */
	UFUNCTION(BlueprintCallable)
	void BroadcastDeactivationAnimationFinished() const;
	
	/** The objective text to be displayed. */
	UPROPERTY(FieldNotify, BlueprintReadWrite, EditAnywhere)
	FText Text;

	/** The VM allows for users to get notified/inform about deactivation animation requests. */
	UPROPERTY(BlueprintAssignable)
	FOnDeactivationAnimationRequested OnDeactivationAnimationRequested;

	/** The VM allows for users to get notified/inform about deactivation animation finishes. */
	FOnDeactivationAnimationFinished OnDeactivationAnimationFinished;
	
protected:
	void HandleOnTextChanged(UObjectiveInstance* InObjective);
};
