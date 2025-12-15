// Fill out your copyright notice in the Description page of Project Settings.


#include "UtillityFunctions.h"


TArray<FVector> UUtillityFunctions::genArray(FVector middle, float range, float step)
{
	TArray<FVector> positions;

	FVector span1 = FVector::CrossProduct(middle, FVector(0, 0, 100)).GetSafeNormal();
	FVector span2 = FVector::CrossProduct(middle, span1).GetSafeNormal();

	for(float i = -range/2; i < range; i = i + step)
	{
		for (float j = -range/2; j < range/4; j = j + step)
			positions.Add(middle + span1 * i + span2 * j);
	}
	
	return positions;
}
