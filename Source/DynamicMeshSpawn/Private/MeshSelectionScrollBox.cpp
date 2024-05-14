// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
    MeshSelectionScrollBox = SNew(SMeshSelectionScrollBox).InMeshAssetManager(MeshAssetManager).ThumbnailSizeScale(ThumbnailSizeScale);
    MeshSelectionScrollBox->OnAssetThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleAssetThumbnailSelected);
    return MeshSelectionScrollBox.ToSharedRef();
}

void UMeshSelectionScrollBox::HandleAssetThumbnailSelected(const FMeshData& MeshData)
{
    OnAssetThumbnailSelected.Broadcast(MeshData);
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
        MeshSelectionScrollBox->ThumbnailSizeScale = ThumbnailSizeScale;
        MeshSelectionScrollBox->RefreshAssetThumbnails();
    }
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MeshSelectionScrollBox.Reset();
}
