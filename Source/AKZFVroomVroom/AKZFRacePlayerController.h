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

	virtual void SetupInputComponent() override;
	
	/* Configures the overlay for the game */
	UFUNCTION(Client, Reliable)
	void ConfigureUI();
	/* Configures the end game menu! */
	UFUNCTION(Client, Reliable)
	void ShowEndGameMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> EndMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widget")
	TSubclassOf<UUserWidget> OverlayClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> PauseMenuClass;

	/* The current user widget */
	UPROPERTY(BlueprintReadWrite, Category="Widget")
	UUserWidget* Overlay;

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	UUserWidget* PauseMenu;

	void ShowPauseMenu();
};
