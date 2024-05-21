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
	isUIHidden = false ;
	idx = 0;

	PawnReference.Add(PerspectivePawn->StaticClass());
	PawnReference.Add(OrthographicPawn->StaticClass());
	PawnReference.Add(IsometricPawn->StaticClass());
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

	SwitchCameraView();
}

void AInteractiveArchController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if (MeshData.StaticMesh)
	{
		FBox BoundingBox = MeshData.StaticMesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;
		float OffsetZ = MaxBounds.Z;
		 
		if (LastHitLocation == CurrentHitLocation) {
			if (CurrentActor) {
				CurrentActor->Destroy();
			}
		}
		else if (AMyStaticMeshActor* StaticMeshActor = Cast<AMyStaticMeshActor>(HitActor)) {
			CurrentHitLocation = StaticMeshActor->GetActorLocation() - FVector(0, 0, OffsetZ);
			StaticMeshActor->Destroy();
		}
		LastHitLocation = CurrentHitLocation;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentActor = GetWorld()->SpawnActor<AMyStaticMeshActor>(CurrentHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters);

		if (CurrentActor)
		{
			CurrentActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			CurrentActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.StaticMesh);
		}

		if (GetPawn())
		{
			GetPawn()->SetActorLocation(CurrentHitLocation + FVector{0, 0, 100});
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

		UInputAction* PossessPawn = NewObject<UInputAction>();
		PossessPawn->ValueType = EInputActionValueType::Boolean;
		InputMappingContext->MapKey(PossessPawn, EKeys::P);

		EnhancedInputComponent->BindAction(ClickAction,ETriggerEvent::Completed, this, &AInteractiveArchController::ProcessMouseClick);
		EnhancedInputComponent->BindAction(ToggleVisibilityOfWidget, ETriggerEvent::Completed, this, &AInteractiveArchController::ToggleVisibility);
		EnhancedInputComponent->BindAction(PossessPawn, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchCameraView);

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
			HitActor = HitResult.GetActor();
			if (HitActor)
			{
				if (AMyStaticMeshActor* StaticMeshActor = Cast<AMyStaticMeshActor>(HitActor)) {
					SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Visible);
					CurrentActor = StaticMeshActor;
					if (GetPawn())
					{
						GetPawn()->SetActorLocation(HitActor->GetActorLocation() + FVector{0, 0, 50});
					}
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

void AInteractiveArchController::SwitchCameraView()
{
	if (idx == PawnReference.Num())
	{
		idx = 0;
	}

	TSubclassOf<APawn> PawnClass = PawnReference[idx++];
	if (PawnClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (GetPawn())
		{
			GetPawn()->Destroy();
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsystem->ClearAllMappings();
		}

		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, LastHitLocation + FVector(0, 0, 200), FRotator::ZeroRotator, SpawnParams);
		
		SetupInputComponent();

		if (SpawnedPawn)
		{
			Possess(SpawnedPawn);
		}
	}
}
