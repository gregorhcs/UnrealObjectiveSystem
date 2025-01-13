// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ObjectiveGameService.h"
#include "UObject/Object.h"
#include "ObjectiveInstance.generated.h"

enum class EObjectiveDeactivationReason : uint8;
class UObjectiveInstance;
class UObjectiveTemplate;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveChanged, UObjectiveInstance*);

/**
 * The objective instance is the online version of an objective that is created
 * when an objective is activated at runtime based on its template.
 */
UCLASS(Blueprintable)
class OBJECTIVESYSTEM_API UObjectiveInstance : public UObject
{
	GENERATED_BODY()

	friend UObjectiveGameService;

public:
	/**
	 * Called when this objective is activated and was just instantiated.
	 * @param InTemplate - the template this objective is based on
	 */
	virtual void Activate(UObjectiveTemplate* InTemplate);
	
	/**
	 * Called when this objective is deactivated and this instance about to be destroyed.
	 * @param DeactivationReason - the reason why the objective was deactivated
	 */
	virtual void Deactivate(EObjectiveDeactivationReason DeactivationReason);

	/**
	 * Construct the current title of the objective.
	 */
	virtual FText GetCurrentTitle();

	/**
	 * Get the tag this objective is associated with.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FGameplayTag& GetTag() const;

	/**
	 * Get the template this objective is based upon.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UObjectiveTemplate* GetTemplate() const;
	
	/**
	 * Delegate that is fired when the title of this objective has changed.
	 */
	FOnObjectiveChanged OnTitleChanged;

protected:
	UPROPERTY(BlueprintReadOnly, Transient)
	UObjectiveTemplate* Template;

private:
	void SetTag(const FGameplayTag& InTag);

	FGameplayTag Tag = FGameplayTag::EmptyTag;
};
