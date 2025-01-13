// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveListVM.h"

#include "ObjectiveGameService.h"
#include "ObjectiveSystem.h"
#include "ObjectiveVM.h"
#include "GameService/GameServiceLocator.h"
#include "ObjectiveTypes/ObjectiveInstance.h"
#include "ObjectiveTypes/ObjectiveTemplate.h"

void UObjectiveListVM::Initialize()
{
	const auto ObjectiveGameService = UGameServiceLocator::FindService<UObjectiveGameService>();
	
	ObjectiveGameService->OnObjectiveActivated.AddUObject(this, &UObjectiveListVM::HandleOnObjectiveActivated);
	ObjectiveGameService->OnObjectiveDeactivated.AddUObject(this, &UObjectiveListVM::HandleOnObjectiveDeactivated);
}

void UObjectiveListVM::HandleOnObjectiveActivated(FGameplayTag ObjectiveID)
{
	if (Visibility == ESlateVisibility::Hidden)
	{
		Visibility = ESlateVisibility::Visible;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Visibility);
	}

	const auto ObjectiveService = UGameServiceLocator::FindService<UObjectiveGameService>();

	const auto VM = NewObject<UObjectiveVM>();
	UObjectiveInstance* Objective = ObjectiveService->GetObjective(ObjectiveID);
		
	VM->Initialize(Objective);

	if (Objective->GetTemplate()->bMinor)
	{
		const int Index = MinorObjectivesVMs.Add(VM);
		MinorObjectivesVMIndices.Add(ObjectiveID, Index);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MinorObjectivesVMs);
	}
	else
	{
		const int Index = MajorObjectivesVMs.Add(VM);
		MajorObjectivesVMIndices.Add(ObjectiveID, Index);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MajorObjectivesVMs);
	}
}

void UObjectiveListVM::HandleOnObjectiveDeactivated(
	FGameplayTag ObjectiveID,
	EObjectiveDeactivationReason DeactivationReason)
{
	if (!MinorObjectivesVMIndices.Contains(ObjectiveID) &&
		!MajorObjectivesVMIndices.Contains(ObjectiveID))
	{
		UE_LOG(LogObjectiveGameService, Error, TEXT("Tried to find objective VM to ID %s but could not!"), *ObjectiveID.ToString())
		return;
	}
	
	const auto ObjectiveService = UGameServiceLocator::FindService<UObjectiveGameService>();
	UObjectiveInstance* Objective = ObjectiveService->GetObjective(ObjectiveID);
	check(IsValid(Objective))

	UObjectiveVM* ObjectiveVM = StaticCast<UObjectiveVM*>(
		Objective->GetTemplate()->bMinor
			? MinorObjectivesVMs[MinorObjectivesVMIndices[ObjectiveID]]
			: MajorObjectivesVMs[MajorObjectivesVMIndices[ObjectiveID]]
		);

	ObjectiveVM
		->OnDeactivationAnimationFinished
		.AddLambda([this, ObjectiveID, Objective]()
		{
			HandleOnEntryDeactivationAnimationFinished(ObjectiveID, Objective->GetTemplate()->bMinor);
		});

	ObjectiveVM
		->OnDeactivationAnimationRequested
		.Broadcast(DeactivationReason);
}

void UObjectiveListVM::HandleOnEntryDeactivationAnimationFinished(FGameplayTag ObjectiveID, bool bMinor)
{
	if (bMinor)
	{
		const int VmIndex = MinorObjectivesVMIndices[ObjectiveID];
		
		StaticCast<UObjectiveVM*>(MinorObjectivesVMs[VmIndex])
			->OnDeactivationAnimationFinished
			.Clear();
		
		MinorObjectivesVMs[VmIndex] = nullptr;
		MinorObjectivesVMIndices.Remove(ObjectiveID);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MinorObjectivesVMs);
	}
	else
	{
		const int VmIndex = MajorObjectivesVMIndices[ObjectiveID];
		
		StaticCast<UObjectiveVM*>(MajorObjectivesVMs[VmIndex])
			->OnDeactivationAnimationFinished
			.Clear();
		
		MajorObjectivesVMs[VmIndex] = nullptr;
		MajorObjectivesVMIndices.Remove(ObjectiveID);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(MajorObjectivesVMs);
	}

	if (MinorObjectivesVMs.IsEmpty() && MajorObjectivesVMs.IsEmpty())
	{
		Visibility = ESlateVisibility::Hidden;
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Visibility);
	}
}
