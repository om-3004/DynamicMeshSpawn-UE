// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FMeshAssetThumbnailSelectedEvent, const FMeshData&);
DECLARE_DELEGATE_OneParam(FMaterialAssetThumbnailSelectedEvent, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FTextureAssetThumbnailSelectedEvent, const FTextureData&);

UCLASS()
class DYNAMICMESHSPAWN_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAssetManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	float ThumbnailSizeScale = 3.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	EAssetType TypeOfAsset;


	FMeshAssetThumbnailSelectedEvent OnMeshAssetThumbnailSelected;
	FMaterialAssetThumbnailSelectedEvent OnMaterialAssetThumbnailSelected;
	FTextureAssetThumbnailSelectedEvent OnTextureAssetThumbnailSelected;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	void HandleMeshAssetThumbnailSelected(const FMeshData& MeshData);
	void HandleMaterialAssetThumbnailSelected(const FMaterialData& MaterialData);
	void HandleTextureAssetThumbnailSelected(const FTextureData& TextureData);

	virtual const FText GetPaletteCategory() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	TSharedPtr<SMeshSelectionScrollBox> MeshSelectionScrollBox;

};
