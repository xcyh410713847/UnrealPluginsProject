// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorModeEdMode.h"
#include "EditorModeEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FEditorModeEdMode::EM_EditorModeEdModeId = TEXT("EM_EditorModeEdMode");

FEditorModeEdMode::FEditorModeEdMode()
{

}

FEditorModeEdMode::~FEditorModeEdMode()
{

}

void FEditorModeEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FEditorModeEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FEditorModeEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FEditorModeEdMode::UsesToolkits() const
{
	return true;
}




