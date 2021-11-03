// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorMode.h"
#include "EditorModeEdMode.h"

#define LOCTEXT_NAMESPACE "FEditorModeModule"

void FEditorModeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FEditorModeEdMode>(FEditorModeEdMode::EM_EditorModeEdModeId, LOCTEXT("EditorModeEdModeName", "EditorModeEdMode"), FSlateIcon(), true);
}

void FEditorModeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FEditorModeEdMode::EM_EditorModeEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorModeModule, EditorMode)