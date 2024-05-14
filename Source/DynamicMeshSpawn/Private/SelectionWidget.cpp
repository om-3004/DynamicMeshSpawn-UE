// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionWidget.h"

void USelectionWidget::ReceiveMeshData(FMeshData MeshData)
{
	OnMeshAssetThumbnailSelected.Broadcast(MeshData);
}

void USelectionWidget::ReceiveMaterialData(FMaterialData MaterialData)
{
	OnMaterialAssetThumbnailSelected.Broadcast(MaterialData);
}

void USelectionWidget::ReceiveTextureData(FTextureData TextureData)
{
	OnTextureAssetThumbnailSelected.Broadcast(TextureData);
}

void USelectionWidget::InitializeWidget(UMeshAssetManager* MeshAssetManager)
{
	OnMeshDataInitialized(MeshAssetManager);
}

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
