// Copyright Epic Games, Inc. All Rights Reserved.

#include "roboExpGameMode.h"
#include "roboExpPlayerController.h"

AroboExpGameMode::AroboExpGameMode()
{
	PlayerControllerClass = AroboExpPlayerController::StaticClass();
}
