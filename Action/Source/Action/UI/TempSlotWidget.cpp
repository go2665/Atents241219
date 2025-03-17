// Fill out your copyright notice in the Description page of Project Settings.


#include "TempSlotWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UTempSlotWidget::UpdateLocation()
{
	FVector2D MousePosition;
	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("MousePosition: %s"), *MousePosition.ToString()));
	Cast<UCanvasPanelSlot>(Slot)->SetPosition(MousePosition);

	//// UInventoryWidget의 위치와 크기를 가져옵니다.
	//FVector2D InventoryWidgetPosition = GetParent()->GetCachedGeometry().GetAbsolutePosition();
	//FVector2D InventoryWidgetSize = GetParent()->GetCachedGeometry().GetLocalSize();

	//// 마우스 위치를 UInventoryWidget의 상대 좌표로 변환합니다.
	//FVector2D RelativeMousePosition = MousePosition - InventoryWidgetPosition;

	//// 변환된 상대 좌표를 사용하여 UTempSlotWidget의 위치를 설정합니다.
	//UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
	//if (CanvasSlot)
	//{
	//	CanvasSlot->SetPosition(RelativeMousePosition);
	//}

}

void UTempSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
	}
}

void UTempSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//FVector2D MousePosition;
	//PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);	
	////Cast<UCanvasPanelSlot>(Slot)->SetPosition(
	////	MyGeometry.AbsoluteToLocal(MousePosition) * UWidgetLayoutLibrary::GetViewportScale(this));
	//UpdateLocation();
}

void UTempSlotWidget::ShowSlotVisibility()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTempSlotWidget::ClearSlotWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}
