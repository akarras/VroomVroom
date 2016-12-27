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
	
	UFUNCTION(Client, Reliable)
	void ConfigureUI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> OverlayClass;

	UPROPERTY(BlueprintReadWrite, Category="Widgets")
	UUserWidget* Overlay;

	void WidgetCleanup();
	
};
