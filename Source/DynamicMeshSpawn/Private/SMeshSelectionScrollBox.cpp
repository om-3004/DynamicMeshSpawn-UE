// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"
//#include "MeshSelectionScrollBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	RootBorder = SNew(SBorder);

	FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.1843137254901961f, 0.23529411764705882f, 0.49411764705882355f, 1.0f));
	//FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.12549f, 0.043137f, 0.101961f, 1.0f));
	ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;

	FSlateBrushOutlineSettings OutlineSettings{};
	OutlineSettings.CornerRadii = FVector4{5.f, 5.f, 5.f, 5.f};
	OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ColorBrush->OutlineSettings = OutlineSettings;
	RootBorder->SetBorderImage(ColorBrush);

	RootVerticalBox = SNew(SVerticalBox);
	RootBoxName = SNew(STextBlock).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FColor::White);
	ScrollBox= SNew(SScrollBox).Orientation(Orient_Horizontal);

	MeshAssetManager = InArgs._InMeshAssetManager;
	ThumbnailSizeScale = InArgs._ThumbnailSizeScale;
	TypeOfAsset = InArgs._InTypeOfAsset;

	RefreshAssetThumbnails();

	RootVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
		[
			RootBoxName.ToSharedRef()
		];

	RootVerticalBox->AddSlot().HAlign(HAlign_Center)
		[
			ScrollBox.ToSharedRef()
		];

	RootBorder->SetContent(RootVerticalBox.ToSharedRef());
	
	ChildSlot
	[
		RootBorder.ToSharedRef()
	];

}

void SMeshSelectionScrollBox::RefreshAssetThumbnails()
{
	switch (TypeOfAsset) {
		case EAssetType::MeshData:
			RefreshMeshAssetThumbnails();
			break;
		case EAssetType::MaterialData:
			RefreshMaterialAssetThumbnails();
			break;
		case EAssetType::TextureData:
			RefreshTextureAssetThumbnails();
			break;
	}
}

void SMeshSelectionScrollBox::RefreshMeshAssetThumbnails()
{
	RootBoxName->SetText(FText::FromString("Mesh Selector"));
	ScrollBox->ClearChildren();
	if (MeshAssetManager.IsValid())
	{
		for (const FMeshData& MeshData : MeshAssetManager->MeshData)
		{
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MeshData.Thumbnail))
			{
				TSharedPtr<STextBlock> MeshName = SNew(STextBlock).Text(FText::FromString(MeshData.MeshName)).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(MeshData.Thumbnail);
				//SlateBrush->ImageSize = FVector2D(ThumbnailTexture->GetSizeX() * ThumbnailSizeScale, ThumbnailTexture->GetSizeY());
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &MeshData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
						if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
							OnMeshAssetThumbnailSelected.ExecuteIfBound(MeshData);
							return FReply::Handled();
						}
						return FReply::Unhandled();
					});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						MeshName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

void SMeshSelectionScrollBox::RefreshMaterialAssetThumbnails()
{
	RootBoxName->SetText(FText::FromString("Material Selector"));
	ScrollBox->ClearChildren();
	if (MeshAssetManager.IsValid())
	{
		for (const FMaterialData& MaterialData : MeshAssetManager->MaterialData)
		{
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(MaterialData.Thumbnail))
			{
				TSharedPtr<STextBlock> MaterialName = SNew(STextBlock).Text(FText::FromString(MaterialData.MaterialName)).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(MaterialData.Thumbnail);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &MaterialData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMaterialAssetThumbnailSelected.ExecuteIfBound(MaterialData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						MaterialName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

void SMeshSelectionScrollBox::RefreshTextureAssetThumbnails()
{
	RootBoxName->SetText(FText::FromString("Texture Selector"));
	ScrollBox->ClearChildren();
	if (MeshAssetManager.IsValid())
	{
		for (const FTextureData& TextureData : MeshAssetManager->TextureData)
		{
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(TextureData.Thumbnail))
			{
				TSharedPtr<STextBlock> TextureName = SNew(STextBlock).Text(FText::FromString(TextureData.TextureName)).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20)).ColorAndOpacity(FSlateColor(FLinearColor(0.946289f, 0.984314f, 0.626183f, 1.0f)));

				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				FSlateBrushOutlineSettings OutlineSettings{};
				OutlineSettings.CornerRadii = FVector4{ 5.f , 5.f ,5.f , 5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				SlateBrush->OutlineSettings = OutlineSettings;

				SlateBrush->SetResourceObject(TextureData.Thumbnail);
				SlateBrush->ImageSize = FVector2D(ThumbnailSizeScale);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, &TextureData](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnTextureAssetThumbnailSelected.ExecuteIfBound(TextureData);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<SBorder> ScrollBoxBorder = SNew(SBorder);
				FSlateColorBrush* ColorBrush = new FSlateColorBrush(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
				ColorBrush->DrawAs = ESlateBrushDrawType::RoundedBox;
				OutlineSettings.CornerRadii = FVector4{ 5.f ,5.f ,5.f ,5.f };
				OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
				ColorBrush->OutlineSettings = OutlineSettings;

				ScrollBoxBorder->SetBorderImage(ColorBrush);
				ScrollBoxBorder->SetPadding(FMargin(1, 1, 1, 1));

				ScrollBoxBorder->SetContent(ThumbnailImage.ToSharedRef());
				TSharedPtr<SVerticalBox> ScrollBoxVerticalBox = SNew(SVerticalBox);

				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).Padding(5)
					[
						ScrollBoxBorder.ToSharedRef()
					];
				ScrollBoxVerticalBox->AddSlot().HAlign(HAlign_Center).AutoHeight()
					[
						TextureName.ToSharedRef()
					];
				ScrollBox->AddSlot()
					[
						ScrollBoxVerticalBox.ToSharedRef()
					];

			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
