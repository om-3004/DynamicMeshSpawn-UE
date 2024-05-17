// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

USTRUCT(Blueprintable)
struct FMeshData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture* Thumbnail;
};

USTRUCT(Blueprintable)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString MaterialName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	float MaterialMetallicity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	float MaterialRoughness;
};

USTRUCT(Blueprintable)
struct FTextureData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	FString TextureName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* Thumbnail;
};

UCLASS(BlueprintType)
class DYNAMICMESHSPAWN_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<FMeshData> MeshData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FMaterialData> MaterialData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	TArray<FTextureData> TextureData;
};