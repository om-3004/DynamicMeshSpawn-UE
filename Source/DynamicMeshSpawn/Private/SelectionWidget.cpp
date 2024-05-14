// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionWidget.h"

void USelectionWidget::ReceiveMeshData(FMeshData MeshData)
{
	OnAssetThumbnailSelected.Broadcast(MeshData);
}

void USelectionWidget::InitialializeWidget(UMeshAssetManager* MeshAssetManager)
{
	OnMeshDataInitialized(MeshAssetManager);
}

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
