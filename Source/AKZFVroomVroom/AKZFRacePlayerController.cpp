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

void AAKZFRacePlayerController::WidgetCleanup()
{
	/*for (TObjectIterator<UUserWidget> itr; itr; ++itr)
	{
		UUserWidget* widget = *itr;
		if (!widget->GetWorld())
		{
			continue;
		}
		else
		{
			widget->RemoveFromParent();
		}
	}*/
}