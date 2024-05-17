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
	isUIHidden = false;
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	if (SelectionWidgetClassRef)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClassRef);
		if (SelectionWidget)
		{
			SelectionWidget->ScrollBox1->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox1->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox1->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);

			SelectionWidget->ScrollBox2->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox2->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox2->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);

			SelectionWidget->ScrollBox3->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox3->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox3->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);
		}
	}
}

void AInteractiveArchController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if (MeshData.StaticMesh)
	{
		if (LastHitLocation == CurrentHitLocation)
		{
			if (CurrentActor)
				CurrentActor->Destroy();
		}

		LastHitLocation = CurrentHitLocation;

		FBox BoundingBox = MeshData.StaticMesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;
		float OffsetZ = MaxBounds.Z;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentActor = GetWorld()->SpawnActor<AMyStaticMeshActor>(CurrentHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters);

		if (CurrentActor)
		{
			CurrentActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			CurrentActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.StaticMesh);
		}
	}
}

void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData)
{
	if (CurrentActor && MaterialData.Material)
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialData.Material, this);

		if (DynamicMaterialInstance)
		{
			UStaticMeshComponent* StaticMeshComponent = CurrentActor->GetStaticMeshComponent();
			StaticMeshComponent->SetMaterial(0, MaterialData.Material);

			//DynamicMaterialInstance->SetScalarParameterValue(FName("Metallic"), MaterialData.MaterialMetallicity);
			//DynamicMaterialInstance->SetScalarParameterValue(FName("Roughness"), MaterialData.MaterialRoughness);

			//float metallicValue;

			//FString FloatMessage = FString::Printf(TEXT("Metallic: %f"), DynamicMaterialInstance->GetScalarParameterValue(FName("Metallic"), metallicValue));
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FloatMessage);

			//FloatMessage = FString::Printf(TEXT("Roughness: %f"), DynamicMaterialInstance->GetScalarParameterValue(FName("Roughness"), metallicValue));
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FloatMessage);
		}

	}
}

void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData)
{
	if (CurrentActor && TextureData.Thumbnail)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentActor->GetStaticMeshComponent()->GetMaterial(0));
		if(!MaterialInstance) {
			MaterialInstance = UMaterialInstanceDynamic::Create(CurrentActor->GetStaticMeshComponent()->GetMaterial(0), this);
		}
		MaterialInstance->SetTextureParameterValue(FName("Texture"), TextureData.Thumbnail);
		CurrentActor->GetStaticMeshComponent()->SetMaterial(0, MaterialInstance);
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

		UInputAction* ToggleVisibilityOfWidget = NewObject<UInputAction>();
		ToggleVisibilityOfWidget->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(ToggleVisibilityOfWidget, EKeys::Tab);

		EnhancedInputComponent->BindAction(ClickAction,ETriggerEvent::Triggered, this, &AInteractiveArchController::ProcessMouseClick);
		EnhancedInputComponent->BindAction(ToggleVisibilityOfWidget, ETriggerEvent::Completed, this, &AInteractiveArchController::ToggleVisibility);

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
					SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Visible);
					CurrentActor = StaticMeshActor;
				}
				else {
					if (SelectionWidget->ScrollBox1->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Hidden);
					}
					else if (SelectionWidget->ScrollBox2->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Hidden);
					}
					else if (SelectionWidget->ScrollBox3->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				CurrentHitLocation = HitResult.Location;
				
				if (SelectionWidget && !SelectionWidget->IsInViewport())
				{
					SelectionWidget->AddToViewport();
				}
			}
		}
	}
}

void AInteractiveArchController::ToggleVisibility()
{
	if (SelectionWidget->IsInViewport())
	{
		if (isUIHidden)
		{
			SelectionWidget->SetVisibility(ESlateVisibility::Visible);
			isUIHidden = false;
		}
		else
		{
			SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
			isUIHidden = true;
		}
	}
}
