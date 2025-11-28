// Copyright Epic Games, Inc. All Rights Reserved.

#include "roboExpWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UroboExpWheelRear::UroboExpWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}