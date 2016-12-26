// Copyright Aaron Karras and Zachary Frye

#include "AKZFVroomVroom.h"
#include "Blueprint/UserWidget.h"
#include "AKZFRacePlayerController.h"

AAKZFRacePlayerController::AAKZFRacePlayerController()
{
	ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintWidget(TEXT("WidgetBlueprint'/Game/VroomVroom/UI/Overlay.Overlay'"));
	if (BlueprintWidget.Object)
	{
		OverlayClass = BlueprintWidget.Object->GeneratedClass;
	}
}

void AAKZFRacePlayerController::BeginPlay()
{
	
}

void AAKZFRacePlayerController::ConfigureUI_Implementation()
{
	WidgetCleanup();
	Overlay = CreateWidget<UUserWidget>(this, OverlayClass);
	Overlay->AddToPlayerScreen();
}

void AAKZFRacePlayerController::WidgetCleanup()
{
	for (TObjectIterator<UUserWidget> itr; itr; ++itr)
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
	}
}