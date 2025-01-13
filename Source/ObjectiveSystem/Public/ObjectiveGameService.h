// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UIRootGameService.h"
#include "GameService/GameServiceBase.h"

#include "ObjectiveGameService.generated.h"

class UObjectiveTemplate;
class UObjectiveInstance_IncreasingCounter;
class UObjectiveInstance;
class UObjectiveSettings;
class UObjectiveDefinitionsDataAsset;
class UUIRootGameService;
enum class EObjectiveDeactivationReason : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveActivatedByService, FGameplayTag)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnObjectiveDeactivatedByService, FGameplayTag, EObjectiveDeactivationReason)

/**
 * Service for managing objectives at runtime.
 */
UCLASS(Config = Game, DefaultConfig, PerObjectConfig, BlueprintType)
class OBJECTIVESYSTEM_API UObjectiveGameService : public UAsyncGameServiceBase
{
	GENERATED_BODY()

public:
	UObjectiveGameService()
	{
		Lifetime = EGameServiceLifetime::ShutdownWithGameInstance;

		// UI root service is used to display the objectives HUD widget.
		ServiceDependencies.Add<UUIRootGameService>();
	}

	// - UAsyncGameServiceBase
	virtual void BeginServiceStart() override;
	virtual void BeginServiceShutdown(bool bIsWorldTearingDown) override;
	// --

	/**
	 * Activates an objective, displaying it on the HUD.
	 *
	 * Entails searching for its template in @ObjectiveDefinitions and, if a template was found,
	 * creating an objective instance from the instance class configured in the template.
	 * Broadcasts @OnObjectiveActivated if successful.
	 * 
	 * @param ObjectiveID - the tag by which a template should be searched in @ObjectiveDefinitions
	 */
	UFUNCTION(BlueprintCallable)
	void ActivateObjective(FGameplayTag ObjectiveID);

	/**
	 * Deactivates an objective, removing it from the HUD.
	 * Broadcasts @OnObjectiveDeactivated if successful.
	 * 
	 * @param ObjectiveID - the objective to deactivate
	 * @param DeactivationReason - the reason for deactivation, influencing the HUD animation
	 */
	UFUNCTION(BlueprintCallable)
	void DeactivateObjective(FGameplayTag ObjectiveID, EObjectiveDeactivationReason DeactivationReason);

	/**
	 * Get the objective instance to @ObjectiveID, if there is any. Returns nullptr otherwise.
	 * @param ObjectiveID - the ID based on which to fetch
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObjectiveInstance* GetObjective(FGameplayTag ObjectiveID);

	/**
	 * Get the objective instance to @ObjectiveID, casted to @UObjectiveInstance_IncreasingCounter,
	 * if there is any and if the cast works. Returns nullptr otherwise.
	 * @param ObjectiveID - the ID based on which to fetch
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UObjectiveInstance_IncreasingCounter* GetObjectiveWithIncreasingCounter(FGameplayTag ObjectiveID);

	/**
	 * Whether the given objective is currently active.
	 * @param ObjectiveID - the objective to check
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsObjectiveActive(const FGameplayTag ObjectiveID);

	// Utility function to log all objective definitions that have been loaded.
	void LogLoadedObjectiveDefinitions();

	// Fired when an objective was activated.
	FOnObjectiveActivatedByService OnObjectiveActivated;
	// Fired when an objective was deactivated.
	FOnObjectiveDeactivatedByService OnObjectiveDeactivated;

protected:
	// Open the objective UI and finish starting this service.
	UFUNCTION()
	void OpenObjectiveUiAndFinishServiceStart();

	// Stores which objectives are currently active and their respective instances.
	UPROPERTY()
	TMap<FGameplayTag, UObjectiveInstance*> ActiveObjectives;

	// UI tag of the objectives root UI.
	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag ObjectiveUiID;

	// Objective data merged from all objective definitions
	UPROPERTY()
	TMap<FGameplayTag, UObjectiveTemplate*> MergedObjectiveData;

	// The pool of objectives that can be managed by this service.
	UPROPERTY(Config, EditDefaultsOnly)
	TArray<TSoftObjectPtr<UObjectiveDefinitionsDataAsset>> ObjectiveDefinitions;

	TArray<FSoftObjectPath> ObjectiveDefinitionsSoftObjectPaths;

	// Used for async loading @ObjectiveDefinitions.
	TSharedPtr<FStreamableHandle> ObjectiveDefinitionsHandle;
};
