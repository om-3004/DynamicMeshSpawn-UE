// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshAssetManager.h"
#include "MeshSelectionScrollBox.h"
#include "SelectionWidget.generated.h"

UCLASS()
class DYNAMICMESHSPAWN_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MeshSelectionScrollBox;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialSelectionScrollBox;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* TextureSelectionScrollBox;
	
	/*UFUNCTION(BlueprintImplementableEvent, Category = "Selection Widget")
	void OnMeshDataInitialized(UMeshAssetManager* MeshAssetManager);

	UPROPERTY(BlueprintAssignable, category = "Selection Widget")
	FMeshAssetThumbnailSelectedEvent OnMeshAssetThumbnailSelected;

	UPROPERTY(BlueprintAssignable, category = "Selection Widget")
	FMaterialAssetThumbnailSelectedEvent OnMaterialAssetThumbnailSelected;

	UPROPERTY(BlueprintAssignable, category = "Selection Widget")
	FTextureAssetThumbnailSelectedEvent OnTextureAssetThumbnailSelected;

	UFUNCTION(BlueprintCallable, Category = "Selection Widget")
	void ReceiveMeshData(FMeshData MeshData);

	UFUNCTION(BlueprintCallable, Category = "Selection Widget")
	void ReceiveMaterialData(FMaterialData MaterialData);

	UFUNCTION(BlueprintCallable, Category = "Selection Widget")
	void ReceiveTextureData(FTextureData TextureData);

	void InitializeWidget(UMeshAssetManager* MeshAssetManager);

	virtual void NativeConstruct() override;*/
};
