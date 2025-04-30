// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "TowerBuildButtonWidget.h"


void UTowerBuilderWidget::OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas)
{
	TowerDatas = InTowerDatas;

	// 반지름은 크기박스 크기의 절반
	float Radius = SizeBox->GetWidthOverride() * 0.5f;

	int32 Count = TowerDatas->Num();
	if (Canvas && Count > 0)
	{
		for (int32 i = 0; i < Count; i++)
		{
			// 각도 계산
			float RadianAngle = FMath::DegreesToRadians(i * (360.0f / Count) - 90.0f);

			// 위치 좌표 구하기
			float X = FMath::Cos(RadianAngle) * Radius;
			float Y = FMath::Sin(RadianAngle) * Radius;

			// 버튼 생성
			UTowerBuildButtonWidget* TowerButtonWidget =
				CreateWidget<UTowerBuildButtonWidget>(this, TowerButtonButtonWidgetClass);
						
			UCanvasPanelSlot* ChildCanvasSlot = Canvas->AddChildToCanvas(TowerButtonWidget);
			ChildCanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));		// 앵커와 피봇을 중앙으로 설정
			ChildCanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

			ChildCanvasSlot->SetPosition(FVector2D(X, Y));
			//UE_LOG(LogTemp, Warning, TEXT("ChildCanvasSlot Position : [%.1f, %.1f]"), X, Y);

			ChildCanvasSlot->SetSize(TowerButtonWidget->GetButtonSize());

			TowerButtonWidget->OnInitialize(i, (*InTowerDatas)[i]->TowerImage, (*InTowerDatas)[i]->TowerCost);
			TowerButtonWidget->OnBuildButtonClicked.BindUFunction(this, FName("OnBuildButtonClicked"));	
		}
	}

	FWidgetAnimationDynamicEvent CloseEvent;
	CloseEvent.BindUFunction(this, FName("OnCloseAnimationFinished"));
	BindToAnimationFinished(CloseAnimation, CloseEvent);

	SetVisibility(ESlateVisibility::Hidden);
}

void UTowerBuilderWidget::Open()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Builder Widget"));

	SetRenderScale(FVector2D(0.0f, 0.0f));
	SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(OpenAnimation);	// 확대 애니메이션 재생
}

void UTowerBuilderWidget::Close()
{
	//SetVisibility(ESlateVisibility::Hidden);
	if (GetRenderTransform().Scale.X > 0)	// 열려있으면
	{
		UE_LOG(LogTemp, Warning, TEXT("Close Builder Widget"));
		PlayAnimation(CloseAnimation);	// 닫는 애니메이션 실행
	}
}

void UTowerBuilderWidget::OnBuildButtonClicked(int32 InIndex)
{
	Close();

	// 버튼이 클릭되면 TowerBuilder에게 요청 전달
	OnTowerBuildRequest.ExecuteIfBound(InIndex);
}

void UTowerBuilderWidget::OnCloseAnimationFinished()
{
	// 다 작아지면 안보이게 설정
	SetVisibility(ESlateVisibility::Hidden);
	UE_LOG(LogTemp, Warning, TEXT("Close Animation Finished"));
}
