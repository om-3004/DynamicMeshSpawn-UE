// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MeshAssetManager.h"

DECLARE_DELEGATE_OneParam(FAssetThumbnailSelected, const FMeshData&)

class DYNAMICMESHSPAWN_API SMeshSelectionScrollBox : public SCompoundWidget
{

	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SBorder> Border;

public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAssetManager)
	SLATE_ARGUMENT(float, ThumbnailSizeScale)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<UMeshAssetManager> MeshAssetManager;
	float ThumbnailSizeScale;
	void RefreshAssetThumbnails();

	FAssetThumbnailSelected OnAssetThumbnailSelected;
};
