// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	ScrollBox = SNew(SScrollBox);

	MeshAssetManager = InArgs._InMeshAssetManager;
	ThumbnailSizeScale = InArgs._ThumbnailSizeScale;


	ChildSlot
	[
		ScrollBox.ToSharedRef()
	];

	ScrollBox->SetOrientation(Orient_Horizontal);
	RefreshAssetThumbnails();
}

void SMeshSelectionScrollBox::RefreshAssetThumbnails()
{
	ScrollBox->ClearChildren();
	if (MeshAssetManager.IsValid())
	{
		for (const FMeshData& MeshData : MeshAssetManager->MeshData)
		{
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MeshData.Thumbnail))
			{
				FSlateBrush* ThumbanailBrush = new FSlateBrush;
				ThumbanailBrush->SetResourceObject(ThumbnailTexture);
				ThumbanailBrush->ImageSize = FVector2D(ThumbnailTexture->GetSizeX() * ThumbnailSizeScale, ThumbnailTexture->GetSizeY());

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbanailBrush).Cursor(EMouseCursor::Hand)
				.OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnAssetThumbnailSelected.ExecuteIfBound(MeshData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
				});

				ScrollBox->AddSlot()[
					ThumbnailImage.ToSharedRef()
				];
			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
