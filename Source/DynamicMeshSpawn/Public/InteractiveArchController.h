// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeshAssetManager.h"
#include "SelectionWidget.h"
#include "InteractiveArchController.generated.h"

/**
 * 
 */
UCLASS()
class DYNAMICMESHSPAWN_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AInteractiveArchController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionWidget")
	TSubclassOf<USelectionWidget> SelectionWidgetClassRef;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	/*UFUNCTION(BlueprintImplementableEvent, Category = "Trace")
	void OnFloorDetected();*/

	UFUNCTION()
	void SpawnMeshFromMeshData(const FMeshData& MeshData);

	UPROPERTY()
	FVector LastHitLocation;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void ProcessMouseClick();
};
