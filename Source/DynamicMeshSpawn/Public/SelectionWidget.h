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
	UMeshSelectionScrollBox* ScrollBox1;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* ScrollBox2;

	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* ScrollBox3;
};
