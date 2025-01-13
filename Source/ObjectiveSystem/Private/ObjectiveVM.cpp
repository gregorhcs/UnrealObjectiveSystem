// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveVM.h"

#include "ObjectiveTypes/ObjectiveInstance.h"
#include "ObjectiveTypes/ObjectiveTemplate.h"

void UObjectiveVM::Initialize(UObjectiveInstance* InObjective)
{
	InObjective->OnTitleChanged.AddUObject(this, &UObjectiveVM::HandleOnTextChanged);
	HandleOnTextChanged(InObjective);
}

void UObjectiveVM::BroadcastDeactivationAnimationFinished() const
{
	OnDeactivationAnimationFinished.Broadcast();
}

void UObjectiveVM::HandleOnTextChanged(UObjectiveInstance* InObjective)
{
	FString WipText = "";
	if (InObjective->GetTemplate()->bOptional)
	{
		WipText += "(Could do) ";
	}
	WipText += InObjective->GetCurrentTitle().ToString();
	
	Text = FText::FromString(WipText);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Text);
}
