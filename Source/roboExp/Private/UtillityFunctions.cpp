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


TArray<FVector> UUtillityFunctions::GenerateConeDirections(
    const FVector& Axis,
    float HalfAngleDeg,
    int32 NumSamples
)
{
    TArray<FVector> Directions;

    FVector Dir = Axis.GetSafeNormal();

    // Build a rotation from X-axis to Dir
    FMatrix AxisRotation = FRotationMatrix::MakeFromX(Dir);

    float CosMin = FMath::Cos(FMath::DegreesToRadians(HalfAngleDeg));

    for (int32 i = 0; i < NumSamples; i++)
    {
        // Uniform cone sampling
        float U = FMath::FRand();
        float V = FMath::FRand();

        float CosTheta = FMath::Lerp(CosMin, 1.0f, U);
        float SinTheta = FMath::Sqrt(1.0f - CosTheta * CosTheta);
        float Phi = 2.0f * PI * V;

        // Local cone direction (X forward)
        FVector LocalDir(
            CosTheta,
            SinTheta * FMath::Cos(Phi),
            SinTheta * FMath::Sin(Phi)
        );

        // Rotate into world space
        FVector WorldDir = AxisRotation.TransformVector(LocalDir);
        Directions.Add(WorldDir);
    }

    return Directions;
}