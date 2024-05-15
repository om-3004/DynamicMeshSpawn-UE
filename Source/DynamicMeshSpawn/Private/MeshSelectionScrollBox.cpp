// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
    MeshSelectionScrollBox = SNew(SMeshSelectionScrollBox).InMeshAssetManager(MeshAssetManager).ThumbnailSizeScale(ThumbnailSizeScale).InTypeOfAsset(TypeOfAsset);
    if (TypeOfAsset == EAssetType::MeshData) {
        MeshSelectionScrollBox->OnMeshAssetThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMeshAssetThumbnailSelected);
    }
    else if (TypeOfAsset == EAssetType::MaterialData) {
        MeshSelectionScrollBox->OnMaterialAssetThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMaterialAssetThumbnailSelected);
    }
    else {
		MeshSelectionScrollBox->OnTextureAssetThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleTextureAssetThumbnailSelected);
    }
    
    return MeshSelectionScrollBox.ToSharedRef();
}

void UMeshSelectionScrollBox::HandleMeshAssetThumbnailSelected(const FMeshData& MeshData)
{
    OnMeshAssetThumbnailSelected.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialAssetThumbnailSelected(const FMaterialData& MaterialData)
{
    OnMaterialAssetThumbnailSelected.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleTextureAssetThumbnailSelected(const FTextureData& TextureData)
{
    OnTextureAssetThumbnailSelected.ExecuteIfBound(TextureData);
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
    return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::SynchronizeProperties()
{
    Super::SynchronizeProperties();
    if (MeshSelectionScrollBox.IsValid())
    {
        MeshSelectionScrollBox->MeshAssetManager = MeshAssetManager;
        MeshSelectionScrollBox->TypeOfAsset = TypeOfAsset;
        MeshSelectionScrollBox->ThumbnailSizeScale = ThumbnailSizeScale;
        MeshSelectionScrollBox->RefreshAssetThumbnails();
    }
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MeshSelectionScrollBox.Reset();
}
