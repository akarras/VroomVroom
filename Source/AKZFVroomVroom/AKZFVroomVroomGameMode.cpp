// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AKZFVroomVroom.h"
#include "AKZFVroomVroomGameMode.h"
#include "AKZFVroomVroomPawn.h"
#include "AKZFVroomVroomHud.h"

AAKZFVroomVroomGameMode::AAKZFVroomVroomGameMode()
{
	DefaultPawnClass = AAKZFVroomVroomPawn::StaticClass();
	HUDClass = AAKZFVroomVroomHud::StaticClass();
}
