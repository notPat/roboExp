// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "UtilityFunctions.generated.h"


/**
 * 
 */
UCLASS()
class ROBOEXP_API UUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Generator")
	static TArray<FVector> genArray(FVector middle, float range, float step);

	UFUNCTION(BlueprintCallable, Category = "Generator")
	static TArray<FVector> genGridArray(FVector middle, int32 area, int32 step);
	
	
};


