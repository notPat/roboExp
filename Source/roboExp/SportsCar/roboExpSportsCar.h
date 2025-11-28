// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "roboExpPawn.h"
#include "roboExpSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class AroboExpSportsCar : public AroboExpPawn
{
	GENERATED_BODY()
	
public:

	AroboExpSportsCar();
};
