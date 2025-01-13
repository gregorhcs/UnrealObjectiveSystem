// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveGameService.h"

#include "ObjectiveDefinitionsDataAsset.h"
#include "ObjectiveListVM.h"
#include "ObjectiveSystem.h"
#include "UIRootGameService.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameService/GameServiceLocator.h"
#include "ObjectiveTypes/ObjectiveInstance_IncreasingCounter.h"

void UObjectiveGameService::BeginServiceStart()
{
	// load objective definitions data assets

	// convert all objective definitions TSoftObjectPtr -> FSoftObjectPtr
	for (TSoftObjectPtr<UObjectiveDefinitionsDataAsset> UnloadedDataAsset : ObjectiveDefinitions)
	{
		const FSoftObjectPath Path = UnloadedDataAsset.ToSoftObjectPath();
		if (!ensure(Path.IsValid()))
		{
			UE_LOG(LogObjectiveGameService, Error,
				TEXT("'Objective Definitions' data asset defined in 'Project Settings > Objective Service' is null!"))
			continue;
		}
		ObjectiveDefinitionsSoftObjectPaths.Add(Path);
	}

	// skip loading if no data asset path was valid
	if (ObjectiveDefinitionsSoftObjectPaths.IsEmpty())
	{
		FinishServiceStart();
		return;
	}

	// request the load and merge objective data when load complete
	ObjectiveDefinitionsHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(ObjectiveDefinitionsSoftObjectPaths, [this]()
	{
		for (TSoftObjectPtr<UObjectiveDefinitionsDataAsset>& DataAsset : ObjectiveDefinitions)
		{
			if (!DataAsset.IsValid())
				continue;
			MergedObjectiveData.Append(DataAsset.Get()->ObjectiveData);
		}
		
		UseGameService(this, UUIRootGameService::StaticClass());
	
		WaitForDependencies(this,
			FOnWaitingFinished::CreateUObject(this, &UObjectiveGameService::OpenObjectiveUiAndFinishServiceStart));
	},
	FStreamableManager::AsyncLoadHighPriority);
}

void UObjectiveGameService::BeginServiceShutdown(bool bIsWorldTearingDown)
{
	FinishServiceShutdown();
}

void UObjectiveGameService::ActivateObjective(const FGameplayTag ObjectiveID)
{
	// skip set phase, if objective already active
	if (ActiveObjectives.Contains(ObjectiveID))
	{
		UE_LOG(LogObjectiveGameService, Log,
			TEXT("Objective with tag '%s' was instructed to activate, but is already active!"), *ObjectiveID.ToString())
		return;
	}

	// skip, if objective service isn't running yet and emit a warning
	if (!IsServiceRunning())
	{
		UE_LOG(LogObjectiveGameService, Error,
			TEXT("Objective with tag '%s' was instructed to activate, but objective service isn't running yet!"), *ObjectiveID.ToString())
		return;	
	}
	
	// find objective template
	UObjectiveTemplate** TemplateResult = MergedObjectiveData.Find(ObjectiveID);
	if (!TemplateResult)
	{
		UE_LOG(LogObjectiveGameService, Log,
			TEXT("Objective with tag '%s' was instructed to activate, but no matching template could be found!"), *ObjectiveID.ToString())
		return;
	}
	UObjectiveTemplate* Template = *TemplateResult;

	// activate
	UObjectiveInstance* Instance = NewObject<UObjectiveInstance>(this, *Template->InstanceClass);
	Instance->SetTag(ObjectiveID);
	Instance->Activate(Template);
	ActiveObjectives.Add(ObjectiveID, Instance);
	
	OnObjectiveActivated.Broadcast(ObjectiveID);
	UE_LOG(LogObjectiveGameService, Log, TEXT("Objective '%s' successfully activated."), *ObjectiveID.ToString())
}

void UObjectiveGameService::DeactivateObjective(const FGameplayTag ObjectiveID, const EObjectiveDeactivationReason DeactivationReason)
{
	// skip set phase, if objective not active
	UObjectiveInstance** InstanceResult = ActiveObjectives.Find(ObjectiveID);
	if (!InstanceResult)
	{
		UE_LOG(LogObjectiveGameService, Log,
			TEXT("Objective with tag '%s' was instructed to deactivate, but isn't active!"), *ObjectiveID.ToString())
		return;
	}

	// deactivate
	OnObjectiveDeactivated.Broadcast(ObjectiveID, DeactivationReason);
	
	(*InstanceResult)->Deactivate(DeactivationReason);
	ActiveObjectives.Remove(ObjectiveID);
	
	UE_LOG(LogObjectiveGameService, Log,
		TEXT("Objective with tag '%s' successfully deactivated with reason %s."),
		*ObjectiveID.ToString(),
		*UEnum::GetValueAsString(DeactivationReason))
}

UObjectiveInstance* UObjectiveGameService::GetObjective(const FGameplayTag ObjectiveID)
{
	return ActiveObjectives.Contains(ObjectiveID) ? ActiveObjectives[ObjectiveID] : nullptr;
}

UObjectiveInstance_IncreasingCounter* UObjectiveGameService::GetObjectiveWithIncreasingCounter(const FGameplayTag ObjectiveID)
{
	return Cast<UObjectiveInstance_IncreasingCounter>(GetObjective(ObjectiveID));
}

bool UObjectiveGameService::IsObjectiveActive(const FGameplayTag ObjectiveID)
{
	return ActiveObjectives.Contains(ObjectiveID);
}

void UObjectiveGameService::LogLoadedObjectiveDefinitions()
{
	UE_LOG(LogObjectiveGameService, Log, TEXT("Dumping loaded objective definitions."))
	for (auto Kvp : MergedObjectiveData)
	{
		UE_LOG(LogObjectiveGameService, Log, TEXT("ID = %s, Title = '%s'"), *Kvp.Key.ToString(), *Kvp.Value->Title.ToString())
	}
}

void UObjectiveGameService::OpenObjectiveUiAndFinishServiceStart()
{
	auto* ObjectiveListVM = NewObject<UObjectiveListVM>();
	ObjectiveListVM->Initialize();
	UseGameService<UUIRootGameService>(this).OpenUI(ObjectiveUiID, ObjectiveListVM, true);
	
	FinishServiceStart();
}
