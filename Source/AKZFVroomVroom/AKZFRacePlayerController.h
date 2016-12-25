// Copyright Aaron Karras and Zachary Frye

#pragma once

#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "AKZFRacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AKZFVROOMVROOM_API AAKZFRacePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAKZFRacePlayerController();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UObject> OverlayClass;

	UPROPERTY(BlueprintReadWrite, Category="Widgets")
	UUserWidget* Overlay;
	
};
