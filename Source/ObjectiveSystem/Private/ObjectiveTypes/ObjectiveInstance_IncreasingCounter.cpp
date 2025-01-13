// Copyright 2024 Thieves of the Night. All Rights Reserved.


#include "ObjectiveTypes/ObjectiveInstance_IncreasingCounter.h"

#include "ObjectiveDeactivationReason.h"
#include "ObjectiveGameService.h"


UObjectiveInstance_IncreasingCounter::UObjectiveInstance_IncreasingCounter()
{
	ServiceDependencies.Add<UObjectiveGameService>();
}

void UObjectiveInstance_IncreasingCounter::Activate(UObjectiveTemplate* InOwningObjective)
{
	Super::Activate(InOwningObjective);

	CounterObjectiveTemplate = StaticCast<UObjectiveTemplate_IncreasingCounter*>(InOwningObjective);
}

FText UObjectiveInstance_IncreasingCounter::GetCurrentTitle()
{
	return FText::FormatNamed(Template->Title,
		TEXT("Counter"), FText::FromString(FString::FromInt(CurrentCounter)),
		TEXT("CounterMax"), FText::FromString(FString::FromInt(CounterObjectiveTemplate->GetCounterMax()))
	);
}

void UObjectiveInstance_IncreasingCounter::IncrementCounter(bool& OutIsAtMax, int& OutNewCount)
{
	if (CurrentCounter + 1 <= CounterObjectiveTemplate->GetCounterMax())
	{
		CurrentCounter++;
	}
	OutNewCount = CurrentCounter;

	OnTitleChanged.Broadcast(this);

	if (IsCounterAtMax())
	{
		OutIsAtMax = true;
		UseGameService<UObjectiveGameService>(this)
			.DeactivateObjective(GetTag(), EObjectiveDeactivationReason::Succeeded);
	}
	
	OutIsAtMax = false;
}

void UObjectiveInstance_IncreasingCounter::IncreaseCounter(int Amount, bool& OutIsAtMax, int& OutNewCount)
{
	CurrentCounter += Amount;
	if (CurrentCounter > CounterObjectiveTemplate->GetCounterMax())
	{
		CurrentCounter = CounterObjectiveTemplate->GetCounterMax();
	}
	OutNewCount = CurrentCounter;

	OnTitleChanged.Broadcast(this);

	if (IsCounterAtMax())
	{
		OutIsAtMax = true;
		UseGameService<UObjectiveGameService>(this)
			.DeactivateObjective(GetTag(), EObjectiveDeactivationReason::Succeeded);
	}

	OutIsAtMax = false;
}

int UObjectiveInstance_IncreasingCounter::GetCurrentCounter() const
{
	return CurrentCounter;
}

bool UObjectiveInstance_IncreasingCounter::IsCounterAtZero() const
{
	return CurrentCounter == 0;
}

bool UObjectiveInstance_IncreasingCounter::IsCounterAtMax() const
{
	return CurrentCounter == CounterObjectiveTemplate->GetCounterMax();
}