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
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	if (SelectionWidgetClassRef)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClassRef);
		if (SelectionWidget)
		{
			SelectionWidget->OnMeshAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SpawnMeshFromMeshData);
		}
	}
}

void AInteractiveArchController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if (MeshData.StaticMesh)
	{
		FBox BoundingBox = MeshData.StaticMesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;
		float OffsetZ = MaxBounds.Z;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AStaticMeshActor* SpawnedMesh = GetWorld()->SpawnActor<AStaticMeshActor>(LastHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters))
		{
			SpawnedMesh->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			SpawnedMesh->GetStaticMeshComponent()->SetStaticMesh(MeshData.StaticMesh);
		}
	}
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
			if (HitResult.GetActor())
			{
				LastHitLocation = HitResult.Location;
				
				if (SelectionWidget && !SelectionWidget->IsInViewport())
				{
					SelectionWidget->AddToViewport();
					SelectionWidget->InitializeWidget(MeshAssetManager);
				}

				//OnFloorDetected();
			}
		}
	}
}
