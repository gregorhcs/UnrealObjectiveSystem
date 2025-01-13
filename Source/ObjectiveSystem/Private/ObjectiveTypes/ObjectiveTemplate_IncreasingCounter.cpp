// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveTypes/ObjectiveTemplate_IncreasingCounter.h"

#include "ObjectiveTypes/ObjectiveInstance_IncreasingCounter.h"

UObjectiveTemplate_IncreasingCounter::UObjectiveTemplate_IncreasingCounter()
{
	InstanceClass = UObjectiveInstance_IncreasingCounter::StaticClass();
}
