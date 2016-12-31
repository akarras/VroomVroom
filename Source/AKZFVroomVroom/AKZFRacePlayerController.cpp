// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Blueprint/UserWidget.h"
#include "AKZFRacePlayerState.h"
#include "AKZFRacePlayerController.h"

AAKZFRacePlayerController::AAKZFRacePlayerController()
{
	
}

void AAKZFRacePlayerController::BeginPlay()
{
	
}

void AAKZFRacePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(FName("Pause"), EInputEvent::IE_Released, this, &AAKZFRacePlayerController::ShowPauseMenu);
}

void AAKZFRacePlayerController::RemoveWidget()
{
	if (Overlay)
	{
		Overlay->RemoveFromParent();
	}
}

void AAKZFRacePlayerController::ConfigureUI_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Configuring UI"));
	// Add that UI to the screen.
	if (OverlayClass)
	{
		Overlay = CreateWidget<UUserWidget>(this, OverlayClass);
		if (Overlay)
		{
			Overlay->AddToPlayerScreen();
		}
	}
}

void AAKZFRacePlayerController::ShowEndGameMenu_Implementation()
{
	if (EndMenuClass)
	{
		if (Overlay)
		{
			Overlay->RemoveFromParent(); // Remove the current overlay from the viewport
		}
		Overlay = CreateWidget<UUserWidget>(this, EndMenuClass);
		if (Overlay)
		{
			Overlay->AddToPlayerScreen();
			bShowMouseCursor = true; // We'll be needing that mouse cursor there.
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("End menu not specified in BP"));
	}
}

void AAKZFRacePlayerController::ShowPauseMenu()
{
	if (PauseMenuClass)
	{
		// Check if the pause menu exists or not
		if (!PauseMenu)
		{
			// Create our widget!
			PauseMenu = CreateWidget<UUserWidget>(this, PauseMenuClass);
		}
		// Do the viewport switchy thing based on if it's in the viewport or not.
		if (PauseMenu->IsInViewport())
		{
			PauseMenu->RemoveFromParent();
			bShowMouseCursor = false; // I'll miss you mr. mouse.
		}
		else
		{
			PauseMenu->AddToPlayerScreen();
			bShowMouseCursor = true; // Awwe yea we need that mouse cursor
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Pause menu not specified in BP"));
	}
}