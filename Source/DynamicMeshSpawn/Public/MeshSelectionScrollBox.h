// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAssetThumbnailSelectedEvent, const FMeshData&, MeshData);

UCLASS()
class DYNAMICMESHSPAWN_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAssetManager;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	float ThumbnailSizeScale = 3.5f;

	UPROPERTY(BlueprintAssignable, Category = "Base Widget")
	FAssetThumbnailSelectedEvent OnAssetThumbnailSelected;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	void HandleAssetThumbnailSelected(const FMeshData& MeshData);

	virtual const FText GetPaletteCategory() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	TSharedPtr<SMeshSelectionScrollBox> MeshSelectionScrollBox;

};
