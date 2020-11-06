#include "UiManager.h"

#include "CommanderStyle.h"
#include "Modules/ModuleManager.h"
#include "ISlateReflectorModule.h"
#include "MultiBoxBuilder.h"
#include "SlateOptMacros.h"
#include "TabManager.h"

#define LOCTEXT_NAMESPACE "STestSuite"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedPtr<class FTabManager> FUiManager::UnrealCommanderTabManager = nullptr;

namespace UnrealCommanderMenu
{
    TSharedRef<FWorkspaceItem> MenuRoot = FWorkspaceItem::NewGroup(NSLOCTEXT("MainWindow", "MenuRoot", "MenuRoot") );
    TSharedRef<FWorkspaceItem> ToolbarsTabs = MenuRoot->AddGroup( NSLOCTEXT("MainWindow", "ToolbarsTabs", "Toolbars Tabs") );
    TSharedRef<FWorkspaceItem> PanelsCategory = ToolbarsTabs->AddGroup( NSLOCTEXT("MainWindow", "PanelsCategory", "Panels") );
    TSharedRef<FWorkspaceItem> DeveloperCategory = MenuRoot->AddGroup( NSLOCTEXT("MainWindow", "DeveloperCategory", "Developer") );
}

void FUiManager::RestoreMainWindow()
{
    // Need to load this module so we have the widget reflector tab available
    FModuleManager::LoadModuleChecked<ISlateReflectorModule>("SlateReflector");
    
    FCommanderStyle::Initialize();
    FCommanderStyle::ReloadTextures();

    FGlobalTabmanager::Get()->RegisterTabSpawner("MainWindow", FOnSpawnTab::CreateStatic( &FUiManager::SpawnMainWindow ) )
        .SetDisplayName(LOCTEXT("UnrealCommanderTab", "Unreal Commander"))
        .SetGroup(UnrealCommanderMenu::MenuRoot);

    TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout( "SlateUnrealCommander_Layout" )
    ->AddArea
    (
        FTabManager::NewArea(1200, 760)
#if PLATFORM_MAC
        ->SetWindow( FVector2D(210, 32), false )
#else
        ->SetWindow( FVector2D(210, 10), false )
#endif
        ->Split
        (
            FTabManager::NewStack()
            ->AddTab( "MainWindow", ETabState::OpenedTab )
            //->SetForegroundTab(FName("MainWindow"))
        )
    );
    
    FGlobalTabmanager::Get()->RestoreFrom( Layout, TSharedPtr<SWindow>() );
}

TSharedRef<SDockTab> FUiManager::SpawnMainWindow(const FSpawnTabArgs& Args)
{
    TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout( "UnrealCommander_Layout" )
    ->AddArea
    (
        // The primary area will be restored and returned as a widget.
        // Unlike other areas it will not get its own window.
        // This allows the caller to explicitly place the primary area somewhere in the widget hierarchy.
        FTabManager::NewPrimaryArea()
        ->SetOrientation(Orient_Vertical)
        ->Split
        (
            //The first cell in the primary area will be occupied by a stack of tabs.
            // They are all opened.
            FTabManager::NewStack()
			->SetHideTabWell(true)
            ->SetSizeCoefficient(0.1f)
            ->AddTab("MainToolbarTab", ETabState::OpenedTab)
        )
        ->Split
        (
            // We can subdivide a cell further by using an additional splitter
            FTabManager::NewSplitter()
            ->SetOrientation( Orient_Horizontal )
            ->SetSizeCoefficient(0.7f)
            ->Split
            (
                FTabManager::NewStack()
                ->SetHideTabWell(true)
                ->SetSizeCoefficient(0.5f)
                ->AddTab("LeftPanelTab", ETabState::OpenedTab)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetHideTabWell(true)
                ->SetSizeCoefficient(0.5f)
                ->AddTab("RightPanelTab", ETabState::OpenedTab)
            )
        )
        ->Split
        (            
            FTabManager::NewStack()
			->SetHideTabWell(true)
            ->SetSizeCoefficient(0.1f)
            ->AddTab("CommandLineTab", ETabState::OpenedTab)
        )
        ->Split
        (            
            FTabManager::NewStack()
			->SetHideTabWell(true)
            ->SetSizeCoefficient(0.1f)
            ->AddTab("CommandButtonsTab", ETabState::OpenedTab)
        )
    );

    TSharedRef<SDockTab> UnrealCommanderTab =
        SNew(SDockTab)
        . TabRole( ETabRole::MajorTab )
        . Label( LOCTEXT("UnrealCommanderTabLabel", "Unreal Commander") )
        . ToolTipText( LOCTEXT( "UnrealCommanderTabToolTip", "Unreal Commander - cross-platform file explorer." ) );

    UnrealCommanderTabManager = FGlobalTabmanager::Get()->NewTabManager(UnrealCommanderTab);
    
    UnrealCommanderTabManager->RegisterTabSpawner("MainToolbarTab", FOnSpawnTab::CreateStatic( &FUiManager::SpawnTab, FName("MainToolbarTab") ) )
        .SetDisplayName( NSLOCTEXT("UnrealCommanderTab", "MainToolbarTab", "Toolbar"))
        .SetGroup(UnrealCommanderMenu::ToolbarsTabs);

    UnrealCommanderTabManager->RegisterTabSpawner("LeftPanelTab", FOnSpawnTab::CreateStatic( &FUiManager::SpawnTab, FName("LeftPanelTab") ) )
        .SetDisplayName( NSLOCTEXT("UnrealCommanderTab", "LeftPanelTab", "Left Panel"))
        .SetGroup(UnrealCommanderMenu::PanelsCategory);
    UnrealCommanderTabManager->RegisterTabSpawner("RightPanelTab", FOnSpawnTab::CreateStatic( &FUiManager::SpawnTab, FName("RightPanelTab") ) )
        .SetDisplayName( NSLOCTEXT("UnrealCommanderTab", "RightPanelTab", "Right Panel"))
        .SetGroup(UnrealCommanderMenu::PanelsCategory);

    UnrealCommanderTabManager->RegisterTabSpawner("CommandLineTab", FOnSpawnTab::CreateStatic( &FUiManager::SpawnTab, FName("CommandLineTab") ) )
        .SetDisplayName( NSLOCTEXT("UnrealCommanderTab", "CommandLineTab", "Commandline"))
        .SetGroup(UnrealCommanderMenu::ToolbarsTabs);
    
    UnrealCommanderTabManager->RegisterTabSpawner("CommandButtonsTab", FOnSpawnTab::CreateStatic( &FUiManager::SpawnTab, FName("CommandButtonsTab") ) )
        .SetDisplayName( NSLOCTEXT("UnrealCommanderTab", "CommandButtonsTab", "Main commands"))
        .SetGroup(UnrealCommanderMenu::ToolbarsTabs);

    FMenuBarBuilder MenuBarBuilder = FMenuBarBuilder( TSharedPtr<FUICommandList>() );
    MenuBarBuilder.AddPullDownMenu(
        NSLOCTEXT("UnrealCommanderTab", "WindowMenuLabel", "Window"),
        FText::GetEmpty(),
        FNewMenuDelegate::CreateSP(UnrealCommanderTabManager.ToSharedRef(), &FTabManager::PopulateTabSpawnerMenu, UnrealCommanderMenu::MenuRoot));

    UnrealCommanderTab->SetContent
    (
        SNew( SVerticalBox )
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            MenuBarBuilder.MakeWidget()
        ]
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        [
            UnrealCommanderTabManager->RestoreFrom(Layout, Args.GetOwnerWindow()).ToSharedRef()
        ]
    );

    return UnrealCommanderTab;
}

TSharedRef<SDockTab> FUiManager::SpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
    // TODO: Configure it all
    if (TabIdentifier == FName("MainToolbarTab"))
    {
        return SNew(SDockTab)
            .Label( LOCTEXT("ToolbarTab", "Toolbar") )
            .ToolTipText( LOCTEXT( "ToolbarTabToolTip", "List of the most important and frequently used commands." ) )
            .Clipping(EWidgetClipping::ClipToBounds)
			.ShouldAutosize(true)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("ToolbarTab", "Toolbar"))                
            ];
    } else if (TabIdentifier == FName("LeftPanelTab"))
    {
        return SNew(SDockTab)
            .Label( LOCTEXT("LeftPanelTab", "Left Panel") )
            .Clipping(EWidgetClipping::ClipToBounds)
            [
                SNew(STextBlock)
                .Text( LOCTEXT("LeftPanelTab", "Left Panel") )                
            ];
    } else if (TabIdentifier == FName("RightPanelTab"))
    {
        return SNew(SDockTab)
            .Label( LOCTEXT("RightPanelTab", "Right Panel") )
            .Clipping(EWidgetClipping::ClipToBounds)
            [
                SNew(STextBlock)
                .Text( LOCTEXT("RightPanelTab", "Right Panel") )                
            ];
    } else if (TabIdentifier == FName("CommandLineTab"))
    {
        return SNew(SDockTab)
            .Label( LOCTEXT("CommandLineTab", "CommandLine") )
            .Clipping(EWidgetClipping::ClipToBounds)
			.ShouldAutosize(true)
            [
                SNew(STextBlock)
                .Text( LOCTEXT("CommandLineTab", "CommandLine") )                
            ];
    } else if (TabIdentifier == FName("CommandButtonsTab"))
    {
        return SNew(SDockTab)
            .Label( LOCTEXT("CommandButtonsTab", "Command Buttons") )
            .Clipping(EWidgetClipping::ClipToBounds)
			.ShouldAutosize(true)
            [
                SNew(STextBlock)
                .Text( LOCTEXT("CommandButtonsTab", "Command Buttons") )                
            ];
    } else
    {        
        ensure(false);
        return SNew(SDockTab);
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE