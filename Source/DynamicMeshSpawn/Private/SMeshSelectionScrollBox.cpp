// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"

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

				TSharedPtr<SVerticalBox> VerticleBox = SNew(SVerticalBox);

				TSharedPtr<SBox> SizeBox = SNew(SBox);
				SizeBox->SetWidthOverride(TAttribute<FOptionalSize>{200.f});
				SizeBox->SetHeightOverride(TAttribute<FOptionalSize>{200.f});
				SizeBox->SetPadding(FMargin{ 5.f });

				FSlateColorBrush* Brush = new FSlateColorBrush(FLinearColor{ 0.208745, 0.788352, 1.0 });
				TSharedPtr<SBorder> BorderBox = SNew(SBorder).BorderImage(Brush);

				
				
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

				TSharedPtr<STextBlock> ThumbnailName = SNew(STextBlock).Text(FText::FromString(MeshData.MeshName)).ColorAndOpacity(FLinearColor{ 0.f, 0.f, 0.f });

				BorderBox->SetContent(ThumbnailImage.ToSharedRef())
				SizeBox->SetContent(BorderBox.ToSharedRef());

				VerticleBox->AddSlot()
				[
					SizeBox.ToSharedRef()
				];
				VerticleBox->AddSlot().HAlign(HAlign_Center).FillHeight(0.2)
				[
					ThumbnailName.ToSharedRef()
				];

				ScrollBox->AddSlot()
				[
					VerticleBox.ToSharedRef()
				];


			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
