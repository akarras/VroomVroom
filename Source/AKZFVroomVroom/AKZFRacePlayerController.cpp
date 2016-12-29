// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Blueprint/UserWidget.h"
#include "AKZFRacePlayerController.h"

AAKZFRacePlayerController::AAKZFRacePlayerController()
{
	
}

void AAKZFRacePlayerController::BeginPlay()
{
	
}

void AAKZFRacePlayerController::ConfigureUI_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("Configuring UI"));
	//WidgetCleanup();
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
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("End menu not specified in BP"));
	}
}