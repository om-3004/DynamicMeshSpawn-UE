// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

#include "Engine/StaticMeshActor.h"

AInteractiveArchController::AInteractiveArchController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	SpawnedMesh = nullptr;
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	if (SelectionWidgetClassRef)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClassRef);
		if (SelectionWidget)
		{
			SelectionWidget->MeshSelectionScrollBox->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->MaterialSelectionScrollBox->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->TextureSelectionScrollBox->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);
		}
	}
}

void AInteractiveArchController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if (MeshData.StaticMesh)
	{
		if (PreviousHitLocation == LastHitLocation)
		{
			if(SpawnedMesh)
				SpawnedMesh->Destroy();
		}

		FBox BoundingBox = MeshData.StaticMesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;
		float OffsetZ = MaxBounds.Z;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedMesh = GetWorld()->SpawnActor<AMyStaticMeshActor>(LastHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters);

		if (SpawnedMesh)
		{
			SpawnedMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			SpawnedMesh->GetStaticMeshComponent()->SetStaticMesh(MeshData.StaticMesh);
		}
	}
}

void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData)
{
	
}

void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData)
{
}

void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* ClickAction = NewObject<UInputAction>();
		ClickAction->ValueType = EInputActionValueType::Boolean;

		InputMappingContext->MapKey(ClickAction, EKeys::LeftMouseButton);
		EnhancedInputComponent->BindAction(ClickAction,ETriggerEvent::Triggered, this, &AInteractiveArchController::ProcessMouseClick);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AInteractiveArchController::ProcessMouseClick()
{
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FHitResult HitResult;
		FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetPawn());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				if (AMyStaticMeshActor* StaticMeshActor = Cast<AMyStaticMeshActor>(HitActor)) {
					SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->MaterialSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->TextureSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
				}
				else {
					SelectionWidget->MeshSelectionScrollBox->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->MaterialSelectionScrollBox->SetVisibility(ESlateVisibility::Hidden);
					SelectionWidget->TextureSelectionScrollBox->SetVisibility(ESlateVisibility::Hidden);
				}
				UPROPERTY()
				PreviousHitLocation = LastHitLocation;
				LastHitLocation = HitResult.Location;
				
				if (SelectionWidget && !SelectionWidget->IsInViewport())
				{
					SelectionWidget->AddToViewport();
				}

				//OnFloorDetected();
			}
		}
	}
}
