// Fill out your copyright notice in the Description page of Project Settings.


#include "TempSlotWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/GameUserSettings.h"

void UTempSlotWidget::UpdateLocation()
{
	CalculateParentOrigin();

	//FVector2D ViewportMousePosition;
	//PlayerController->GetMousePosition(ViewportMousePosition.X, ViewportMousePosition.Y);	// 뷰포트 기준 마우스 위치
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, 
	// FString::Printf(TEXT("ViewportMousePosition: %s"), *ViewportMousePosition.ToString()));
		
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Update"));

	FVector2D MousePosition;	// 마우스 좌표(해상도 기준)
	if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, MousePosition.X, MousePosition.Y))	// 해상도 기준 마우스 위치
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, 
		//	FString::Printf(TEXT("MousePosition: %s"), *MousePosition.ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, 
		//	FString::Printf(TEXT("Diff: %s"), *(MousePosition - ParentOrigin).ToString()));

		//UE_LOG(LogTemp, Warning, TEXT("MousePosition: %s"), *MousePosition.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Diff: %s"), *(MousePosition - ParentOrigin).ToString());

		CanvasSlot->SetPosition(MousePosition - ParentOrigin);
	}
}

void UTempSlotWidget::CalculateParentOrigin()
{
	// 첫번째 틱에서 한번만 실행
	if (!bIsCalculatedParentOrigin)
	{
		// 화면 해상도 구하기
		UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
		FVector2D ScreenResolution = GameUserSettings->GetScreenResolution();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, 
		//	FString::Printf(TEXT("ScreenResolution: %s"), *ScreenResolution.ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("ScreenResolution: %s"), *ScreenResolution.ToString());

		// 뷰포트 사이즈 구하기
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
		//GEngine->GameViewport->GetViewportSize(ViewportSize);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,
		//	FString::Printf(TEXT("ViewportSize: %s"), *ViewportSize.ToString()));
		//UE_LOG(LogTemp, Warning, TEXT("ViewportSize: %s"), *ViewportSize.ToString());

		// 해상도에 따른 위치 조정 정도
		float ResolutionAdjustment = 0.0f;

		// 해상도에 따른 위치 조정
		if (ViewportSize.Y > 0)
		{
			float ScaleRatio = ScreenResolution.Y / ViewportSize.Y;	// 해상도 비율에 맞게 스케일
			float ScaledWidth = ViewportSize.X * ScaleRatio;		// 스케일된 너비 구하기

			ResolutionAdjustment = ScaledWidth - ScreenResolution.X;	// 원래 해상도 비율과의 차이 구하기
			//UE_LOG(LogTemp, Warning, TEXT("ResolutionAdjustment: %f"), ResolutionAdjustment);
		}

		// 인벤토리 위젯의 위치와 크기 구하기
		FVector2D ParentPosition = ParentCanvasSlot->GetPosition();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, 
		// FString::Printf(TEXT("InventoryWidgetPosition: %s"), *ParentPosition.ToString()));
		FVector2D ParentSize = ParentCanvasSlot->GetSize();
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, 
		// FString::Printf(TEXT("InventoryWidgetSize: %s"), *ParentSize.ToString()));

		// 부모의 좌상단 위치를 원점으로 사용(인벤토리의 앵커가 우하로 되어 있어서 아래와 같은 수식이 나옴)
		ParentOrigin = ScreenResolution + ParentPosition - ParentSize + FVector2D(ResolutionAdjustment, 0);
		//UE_LOG(LogTemp, Warning, TEXT("ParentOrigin: %s"), *ParentOrigin.ToString());

		bIsCalculatedParentOrigin = true;
	}
}

void UTempSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
	}

	// 캔버스 슬롯 캐스팅 해 놓기
	CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
}

void UTempSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Tick"));
	UpdateLocation();	
}

void UTempSlotWidget::ShowSlotVisibility()
{
	UpdateLocation();	// 위치 이동 시키고 보여주기
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTempSlotWidget::ClearSlotWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	//SetVisibility(ESlateVisibility::HitTestInvisible);
}
