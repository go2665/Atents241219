// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDetailWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/GameUserSettings.h"

void UItemDetailWidget::Open(UItemDataAsset* InData)
{
	if (InData)
	{
		ItemIconImage->SetBrushFromTexture(InData->ItemIcon);
		ItemNameTextBlock->SetText(InData->ItemName);
		ItemPriceTextBlock->SetText(FText::AsNumber(InData->ItemPrice));
		ItemDescriptionTextBlock->SetText(InData->ItemDescription);

		UpdateLocation();	// 위치 갱신

		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UItemDetailWidget::Close()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UItemDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
	}
	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);

	Close();
}

void UItemDetailWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateLocation();
}

void UItemDetailWidget::UpdateLocation()
{
	CalculateParentOrigin();
	FVector2D MousePosition;	// 마우스 좌표(해상도 기준)
	if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, MousePosition.X, MousePosition.Y))	// 해상도 기준 마우스 위치
	{
		//UE_LOG(LogTemp, Warning, TEXT("Diff: %s"), *(MousePosition - ParentOrigin).ToString());
		CanvasSlot->SetPosition(MousePosition - ParentOrigin);
	}
}

void UItemDetailWidget::CalculateParentOrigin()
{
	if (!bIsCalculatedParentOrigin)
	{
		// 화면 해상도 구하기
		UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
		FVector2D ScreenResolution = GameUserSettings->GetScreenResolution();

		// 뷰포트 사이즈 구하기
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

		// 해상도에 따른 위치 조정 정도
		float ResolutionAdjustment = 0.0f;

		// 해상도에 따른 위치 조정
		if (ViewportSize.Y > 0)
		{
			float ScaleRatio = ScreenResolution.Y / ViewportSize.Y;	// 해상도 비율에 맞게 스케일
			float ScaledWidth = ViewportSize.X * ScaleRatio;		// 스케일된 너비 구하기

			ResolutionAdjustment = ScaledWidth - ScreenResolution.X;	// 원래 해상도 비율과의 차이 구하기
		}

		// 인벤토리 위젯의 위치와 크기 구하기
		FVector2D ParentPosition = ParentCanvasSlot->GetPosition();
		FVector2D ParentSize = ParentCanvasSlot->GetSize();

		// 부모의 좌상단 위치를 원점으로 사용(인벤토리의 앵커가 우하로 되어 있어서 아래와 같은 수식이 나옴)
		ParentOrigin = ScreenResolution + ParentPosition - ParentSize + FVector2D(ResolutionAdjustment, 0);
		UE_LOG(LogTemp, Warning, TEXT("ParentOrigin: %s"), *ParentOrigin.ToString());

		bIsCalculatedParentOrigin = true;
	}
}
