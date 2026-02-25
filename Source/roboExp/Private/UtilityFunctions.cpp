// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityFunctions.h"


TArray<FVector> UUtilityFunctions::genArray(FVector middle, float range, float step)
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

TArray<FVector> UUtilityFunctions::genGridArray(FVector middle, int32 maxX, int32 minX, int32 maxY, int32 minY, int32 maxZ, int32 minZ, int32 step, int32 stepZ)
{
	TArray<FVector> positions;

	for (float i = -minX; i < maxX; i = i + step)
		for (float j = -minY; j < maxY; j = j + step)
			for(float k = -minZ ; k < maxZ; k = k + stepZ)
				positions.Add(FVector(i, j, k) + middle);
	

	return positions;
}

bool UUtilityFunctions::WriteToFile(const FString& FileName, const FString& Content)
{
	FString FullPath = FPaths::ProjectSavedDir() + "Exports/" + FileName;

	return FFileHelper::SaveStringToFile(Content, *FullPath);
}

bool UUtilityFunctions::WriteLidarCloudToFile(ULidarPointCloud* Cloud, const FString& FullPath)
{
	if (!Cloud) return false;



	return Cloud->Export(FullPath);
}


bool UUtilityFunctions::ExportXYZ(const FString& FullPath, const TArray<FLidarPointCloudPoint>& Points, float UnitScale = 1.0f)
{
	// create folder
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(FullPath), true);

	FString Out;
	Out.Reserve(Points.Num() * 32);

	for (const FLidarPointCloudPoint& P : Points)
	{
		
		float X = P.Location.X;
		float Y = P.Location.Y;
		float Z = P.Location.Z;

		Out += FString::Printf(TEXT("%.3f %.3f %.3f %d %d %d\n"), X, Y, Z, P.Color.R, P.Color.G, P.Color.G);
	}

	return FFileHelper::SaveStringToFile(Out, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8);
}


bool UUtilityFunctions::ImportXYZ(const FString& FullPath, TArray<FLidarPointCloudPoint>& Points, float UnitScale = 1.0f)
{
	Points.Reset();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	TUniquePtr<IFileHandle> FileHandle(PlatformFile.OpenRead(*FullPath));
	if (!FileHandle) return false;

	FString FileContent;
	if (!FFileHelper::LoadFileToString(FileContent, *FullPath))
		return false;

	TArray<FString> Lines;
	FileContent.ParseIntoArrayLines(Lines);

	for (const FString& Line : Lines)
	{
		TArray<FString> Tokens;
		Line.ParseIntoArrayWS(Tokens);

		if (Tokens.Num() < 3) continue;

		const float X = FCString::Atof(*Tokens[0]) * UnitScale;
		const float Y = FCString::Atof(*Tokens[1]) * UnitScale;
		const float Z = FCString::Atof(*Tokens[2]) * UnitScale;

		FLidarPointCloudPoint P(FVector3f(X, Y, Z));

		if (Tokens.Num() >= 6)
		{
			const uint8 R = FCString::Atoi(*Tokens[3]);
			const uint8 G = FCString::Atoi(*Tokens[4]);
			const uint8 B = FCString::Atoi(*Tokens[5]);

			P.Color = FColor(R, G, B, 255);
			
		}

		Points.Add(P);

	}

	return true;
}
