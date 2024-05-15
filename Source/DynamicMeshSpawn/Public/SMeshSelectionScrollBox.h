// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MeshAssetManager.h"

DECLARE_DELEGATE_OneParam(FMeshAssetThumbnailSelected, const FMeshData&)
DECLARE_DELEGATE_OneParam(FMaterialAssetThumbnailSelected, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FTextureAssetThumbnailSelected, const FTextureData&)

UENUM()
enum class EAssetType : uint8 {
	MeshData UMETA(DisplayName = "Mesh Data"),
	MaterialData UMETA(DisplayName = "Material Data"),
	TextureData UMETA(DisplayName = "Texture Data")
};

class DYNAMICMESHSPAWN_API SMeshSelectionScrollBox : public SCompoundWidget
{

	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SBorder> Border;

public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAssetManager)
	SLATE_ARGUMENT(float, ThumbnailSizeScale)
	SLATE_ARGUMENT(EAssetType, InTypeOfAsset)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager;
	float ThumbnailSizeScale;
	EAssetType TypeOfAsset;
	void RefreshAssetThumbnails();
	void RefreshMeshAssetThumbnails();
	void RefreshMaterialAssetThumbnails();
	void RefreshTextureAssetThumbnails();
	 
	FMeshAssetThumbnailSelected OnMeshAssetThumbnailSelected;
	FMaterialAssetThumbnailSelected OnMaterialAssetThumbnailSelected;
	FTextureAssetThumbnailSelected OnTextureAssetThumbnailSelected;
};
