// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealCommanderApp.h"
#include "Runtime/Launch/Public/RequiredProgramMainCPPInclude.h"	// Include relative path from Engine dir
#include "Widgets/Testing/STestSuite.h"
#include "ISourceCodeAccessModule.h"
#include "Widgets/Testing/SPerfSuite.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Application/SlateApplication.h"

IMPLEMENT_APPLICATION(UnrealCommander, "UnrealCommander");

#define LOCTEXT_NAMESPACE "UnrealCommander"

namespace WorkspaceMenu
{
	TSharedRef<FWorkspaceItem> DeveloperMenu = FWorkspaceItem::NewGroup(LOCTEXT("DeveloperMenu", "Developer"));
}

int RunUnrealCommander(const TCHAR* Commandline) 
{
	// start up the main loop
	GEngineLoop.PreInit(Commandline);

	// Make sure all UObject classes are registered and default properties have been initialized
	ProcessNewlyLoadedUObjects();
	
	// Tell the module manager it may now process newly-loaded UObjects when new C++ modules are loaded
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();

	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());

	// Load the source code access module
	ISourceCodeAccessModule& SourceCodeAccessModule = FModuleManager::LoadModuleChecked<ISourceCodeAccessModule>( FName( "SourceCodeAccess" ) );
	
	// set the application name
	FGlobalTabmanager::Get()->SetApplicationTitle(LOCTEXT("AppTitle", "Unreal Commander"));
	FModuleManager::LoadModuleChecked<ISlateReflectorModule>("SlateReflector").RegisterTabSpawner(WorkspaceMenu::DeveloperMenu);
	
	if (FParse::Param(FCommandLine::Get(), TEXT("perftest")))
	{
		// Bring up perf test
		SummonPerfTestSuite();
	}
	else
	{
		// Bring up the test suite.
		RestoreSlateTestSuite();
	}


#if WITH_SHARED_POINTER_TESTS
	SharedPointerTesting::TestSharedPointer<ESPMode::Fast>();
	SharedPointerTesting::TestSharedPointer<ESPMode::ThreadSafe>();
#endif

	// loop while the server does the rest
	while (!IsEngineExitRequested())
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FStats::AdvanceFrame(false);
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();		
		FPlatformProcess::Sleep(0);
	}

	FCoreDelegates::OnExit.Broadcast();
	FSlateApplication::Shutdown();
	FModuleManager::Get().UnloadModulesAtShutdown();


	return 0;
}

#undef LOCTEXT_NAMESPACE
