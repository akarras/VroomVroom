// Fill out your copyright notice in the Description page of Project Settings.

#include "AKZFVroomVroom.h"
#include "AKZFMenuController.h"
#include "AKZFMenuPawn.h"
#include "AKZFMenuGamemode.h"


AAKZFMenuGamemode::AAKZFMenuGamemode()
{
	PlayerControllerClass = AAKZFMenuController::StaticClass();
	DefaultPawnClass = AAKZFMenuPawn::StaticClass();
}

