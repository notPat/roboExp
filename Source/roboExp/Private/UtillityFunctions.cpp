// Fill out your copyright notice in the Description page of Project Settings.


#include "UtillityFunctions.h"


TArray<FVector> UUtillityFunctions::genArray(FVector middle, float range, float step)
{
	TArray<FVector> positions;

	

	FVector span1 = FVector::CrossProduct(middle, FVector(0, 0, 100)).GetSafeNormal();
	FVector span2 = FVector::CrossProduct(middle, span1).GetSafeNormal();

	for(float i = -range; i < range; i = i + step)
	{
		for (float j = -range; j < range/4; j = j + step)
			positions.Add(middle + span1 * FMath::FRandRange(-range/2,range/2) + span2 * FMath::FRandRange(-range/2, range/2));
	}
	
	return positions;
}

TArray<FVector> UUtillityFunctions::genGridArray(FVector middle, int32 area, int32 step)
{
	TArray<FVector> positions;

	for (float i = -area; i < area; i = i + step)
	{
		for (float j = -area; j < area; j = j + step)
			positions.Add(FVector(i, j, 0) + middle);
	}

	return positions;
}
