// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "AKZFVroomVroomHud.generated.h"


UCLASS(config = Game)
class AAKZFVroomVroomHud : public AHUD
{
	GENERATED_BODY()

public:
	AAKZFVroomVroomHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
