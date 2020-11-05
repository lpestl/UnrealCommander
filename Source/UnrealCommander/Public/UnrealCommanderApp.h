// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISlateReflectorModule.h"
#include "UnrealCommander.h"
#include "Widgets/Docking/SDockTab.h"


/**
 * Run the UnrealCommander .
 */
int RunUnrealCommander(const TCHAR* Commandline);

/**
 * Spawn the contents of the web browser tab
 */
TSharedRef<SDockTab> SpawnWebBrowserTab(const FSpawnTabArgs& Args);
