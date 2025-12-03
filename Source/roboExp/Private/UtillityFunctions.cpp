// Fill out your copyright notice in the Description page of Project Settings.


#include "UtillityFunctions.h"


TArray<FVector> UUtillityFunctions::genArray(FVector middle)
{
	TArray<FVector> positions;

	FVector span1 = FVector::CrossProduct(middle, FVector(0, 0, 100)).GetSafeNormal();
	FVector span2 = FVector::CrossProduct(middle, span1).GetSafeNormal();
	
	for(float i = -500.0f; i < 2500.0f; i = i + 500.0f)
	{
		for (float j = -500.0f; j < 2500.0f; j = j + 500.0f)
			positions.Add(middle + span1 * i + span2 * j);
	}
	
	return positions;
}
