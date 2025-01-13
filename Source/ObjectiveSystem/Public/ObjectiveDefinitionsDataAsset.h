// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ObjectiveDefinitionsDataAsset.generated.h"

class UObjectiveTemplate;

/**
 * Data asset defining a list of objective templates whose tags can then be
 * used to manage them via the @UObjectiveGameService.
 */
UCLASS()
class OBJECTIVESYSTEM_API UObjectiveDefinitionsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * Map of gameplay tags identifying objectives to instanced objective templates.
	 * Objectives defined here can be managed via the @UObjectiveGameService.
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TMap<FGameplayTag, UObjectiveTemplate*> ObjectiveData;
};
