// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorStandaloneWindow.h"
#include "EditorStandaloneWindowStyle.h"
#include "EditorStandaloneWindowCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName EditorStandaloneWindowTabName("EditorStandaloneWindow");

#define LOCTEXT_NAMESPACE "FEditorStandaloneWindowModule"

void FEditorStandaloneWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorStandaloneWindowStyle::Initialize();
	FEditorStandaloneWindowStyle::ReloadTextures();

	FEditorStandaloneWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorStandaloneWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorStandaloneWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorStandaloneWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorStandaloneWindowTabName, FOnSpawnTab::CreateRaw(this, &FEditorStandaloneWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorStandaloneWindowTabTitle", "EditorStandaloneWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorStandaloneWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorStandaloneWindowStyle::Shutdown();

	FEditorStandaloneWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorStandaloneWindowTabName);
}

TSharedRef<SDockTab> FEditorStandaloneWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEditorStandaloneWindowModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EditorStandaloneWindow.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FEditorStandaloneWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorStandaloneWindowTabName);
}

void FEditorStandaloneWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorStandaloneWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorStandaloneWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorStandaloneWindowModule, EditorStandaloneWindow)