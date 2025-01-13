// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ObjectiveTemplate.generated.h"

class UObjectiveInstance;

/**
 * The objective template is the offline version of an objective
 * configured in the editor.
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew, DisplayName="Objective")
class OBJECTIVESYSTEM_API UObjectiveTemplate : public UObject
{
	GENERATED_BODY()

public:
	UObjectiveTemplate();

	// The title of the objective to be displayed in the UI.
	UPROPERTY(EditDefaultsOnly)
	FText Title = {};

	// Whether this objective is optional to fulfill.
	UPROPERTY(EditDefaultsOnly)
	bool bOptional = false;

	// Whether this objective is of small relevance.
	UPROPERTY(EditDefaultsOnly)
	bool bMinor = false;

	// When the objective is instantiated, the instance will be created from this class.
	UPROPERTY()
	TSubclassOf<UObjectiveInstance> InstanceClass;
};
