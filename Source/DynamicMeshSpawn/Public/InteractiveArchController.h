// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeshAssetManager.h"
#include "SelectionWidget.h"
#include "MyStaticMeshActor.h"
#include "Engine/DataTable.h"

#include "CameraViews/PerspectiveViewPawn.h"
#include "CameraViews/OrthographicViewPawn.h"
#include "CameraViews/IsometricViewPawn.h"

#include "InteractiveArchController.generated.h"

UENUM()
enum class EPawnType {
	PerspectiveView,
	OrthographicView,
	IsometricView
};

USTRUCT(Blueprintable)
struct FPawnDataTable: public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPawnType PawnType{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass{};
};


UCLASS()
class DYNAMICMESHSPAWN_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()

	AMyStaticMeshActor* SpawnedMesh;
	APerspectiveViewPawn* PerspectivePawn;
	AOrthographicViewPawn* OrthographicPawn;
	AIsometricViewPawn* IsometricPawn;

	TArray<TSubclassOf<APawn>> PawnReference;

	UPROPERTY(VisibleDefaultsOnly, meta = (AlloPrivateAccess = "true"))
	int8 idx;

	AActor* HitActor;
	
protected:
	AInteractiveArchController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionWidget")
	TSubclassOf<USelectionWidget> SelectionWidgetClassRef;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	UFUNCTION()
	void SpawnMeshFromMeshData(const FMeshData& MeshData);
	void ApplyMaterial(const FMaterialData& MaterialData);
	void ApplyTexture(const FTextureData& TextureData);

	UPROPERTY()
	FVector CurrentHitLocation;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	AMyStaticMeshActor* CurrentActor;

	UPROPERTY()
	bool isUIHidden;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void ProcessMouseClick();
	void ToggleVisibility();
	void SwitchCameraView();
};
