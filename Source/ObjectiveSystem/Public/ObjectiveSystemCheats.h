// Copyright 2024 Thieves of the Night. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectiveGameService.h"
#include "Cheat/CheatCommand.h"
#include "GameService/GameServiceLocator.h"

DEFINE_CHEAT_COLLECTION(ExampleCheats, AsCheatMenuTab("Objectives"))
{
	DEFINE_CHEAT_COMMAND(DumpDefinitionsCheat, "Cheat.Objectives.LogDefinitions")
	.DisplayAs("Log Definitions")
	.DescribeCheat("Dump all objective definitions that have been loaded to the log.")
	DEFINE_CHEAT_EXECUTE(DumpDefinitionsCheat)
	{
		UObjectiveGameService* ObjectiveService = UGameServiceLocator::FindService<UObjectiveGameService>();
		if (!IsValid(ObjectiveService))
			return;
		
		ObjectiveService->LogLoadedObjectiveDefinitions();
	}
}
