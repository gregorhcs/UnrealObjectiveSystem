// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveTypes/ObjectiveInstance.h"

#include "ObjectiveTypes/ObjectiveTemplate.h"

void UObjectiveInstance::Activate(UObjectiveTemplate* InTemplate)
{
	Template = InTemplate;
}

void UObjectiveInstance::Deactivate(EObjectiveDeactivationReason DeactivationReason)
{
}

FText UObjectiveInstance::GetCurrentTitle()
{
	check(IsValid(Template))
	return Template->Title;
}

const FGameplayTag& UObjectiveInstance::GetTag() const
{
	check(Tag.IsValid())
	return Tag;
}

const UObjectiveTemplate* UObjectiveInstance::GetTemplate() const
{
	check(IsValid(Template))
	return Template;
}

void UObjectiveInstance::SetTag(const FGameplayTag& InTag)
{
	check(InTag.IsValid())
	Tag = InTag;
}
