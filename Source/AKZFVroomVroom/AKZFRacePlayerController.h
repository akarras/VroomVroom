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
	
	/* Configures the overlay for the game */
	UFUNCTION(Client, Reliable)
	void ConfigureUI();

	UFUNCTION(Client, Reliable)
	void ShowEndGameMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> EndMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UUserWidget> OverlayClass;

	/* The current user widget */
	UPROPERTY(BlueprintReadWrite, Category="Widgets")
	UUserWidget* Overlay;
	
};
