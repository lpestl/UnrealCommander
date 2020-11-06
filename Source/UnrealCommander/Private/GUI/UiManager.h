#pragma once

#include "CoreMinimal.h"
#include "SDockTab.h"

class FUiManager
{
public:
    static void RestoreMainWindow();
    
    static TSharedRef<class SDockTab> SpawnMainWindow( const FSpawnTabArgs& Args );
    
private:
    static TSharedRef<class SDockTab> SpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);
    
private:
    static TSharedPtr<class FTabManager> UnrealCommanderTabManager;
};
