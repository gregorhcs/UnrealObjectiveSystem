// Copyright 2024 Thieves of the Night. All Rights Reserved.

#include "ObjectiveTypes/ObjectiveTemplate.h"

#include "ObjectiveTypes/ObjectiveInstance.h"

UObjectiveTemplate::UObjectiveTemplate()
{
	InstanceClass = UObjectiveInstance::StaticClass();
}
