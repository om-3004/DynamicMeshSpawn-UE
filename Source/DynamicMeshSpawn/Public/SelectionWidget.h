// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshAssetManager.h"
#include "MeshSelectionScrollBox.h"
#include "SelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICMESHSPAWN_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Selection Widget")
	void OnMeshDataInitialized(UMeshAssetManager* MeshAssetManager);

	UPROPERTY(BlueprintAssignable, category = "Selection Widget")
	FAssetThumbnailSelectedEvent OnAssetThumbnailSelected;
	
	UFUNCTION(BlueprintCallable, Category = "Selection Widget")
	void ReceiveMeshData(FMeshData MeshData);

	void InitialializeWidget(UMeshAssetManager* MeshAssetManager);

	virtual void NativeConstruct() override;
};
