// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "LidarPointCloud.h"
#include "Misc/FileHelper.h"
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
	static TArray<FVector> genGridArray(FVector middle, int32 maxX, int32 minX, int32 maxY, int32 minY, int32 maxZ, int32 minZ, int32 step);
	
	UFUNCTION(BlueprintCallable, Category = "Data IO")
	static bool WriteToFile(const FString& FileName, const FString& Content);
	
	UFUNCTION(BlueprintCallable, Category = "Data IO")
	static bool WriteLidarCloudToFile(
		ULidarPointCloud* Cloud,
		const FString& FullPath
	);

	UFUNCTION(BlueprintCallable, Category = "Data IO")
	static bool ExportXYZ(const FString& FullPath, const TArray<FLidarPointCloudPoint>& Points, float UnitScale);

	UFUNCTION(BlueprintCallable, Category = "Data IO")
	static bool ImportXYZ(const FString& FullPath, TArray<FLidarPointCloudPoint>& Points, float UnitScale);

};


