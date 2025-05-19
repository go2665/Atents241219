// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuilderWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "TowerBuildButtonWidget.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"


void UTowerBuilderWidget::OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas)
{
	// 반지름은 크기박스 크기의 절반(버튼 배치에 사용되는 반지름)
	float Radius = SizeBox->GetWidthOverride() * 0.5f;
		
	int32 Count = InTowerDatas->Num();	// 버튼 갯수
	if (Count > 0) // 빌더가 만들 수 있는 타워가 1개 이상일 때만 실행
	{
		// 버튼 갯수만큼 버튼 생성
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

			ChildCanvasSlot->SetPosition(FVector2D(X, Y));	// 버튼 위치 설정
			//UE_LOG(LogTemp, Warning, TEXT("ChildCanvasSlot Position : [%.1f, %.1f]"), X, Y);

			ChildCanvasSlot->SetSize(TowerButtonWidget->GetButtonSize());	// 버튼 크기 설정

			// 버튼의 인덱스와 표시될 이미지와 비용 설정
			TowerButtonWidget->OnInitialize(i, (*InTowerDatas)[i]->TowerImage, (*InTowerDatas)[i]->TowerCost);
			TowerButtonWidget->OnBuildButtonClicked.BindUObject(this, &UTowerBuilderWidget::OnBuildButtonClicked);

			BuildButtons.Add(TowerButtonWidget);	// 버튼을 배열에 추가
		}
	}

	// 애니메이션이 끝날 때 호출될 함수 바인딩
	FWidgetAnimationDynamicEvent CloseEvent;
	CloseEvent.BindUFunction(this, FName("OnCloseAnimationFinished"));
	BindToAnimationFinished(CloseAnimation, CloseEvent);

	// 게임 모드에서 골드가 변경될 때 현재 골드 저장
	UWorld* World = GetWorld();
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(World->GetAuthGameMode());
	if (GameMode)
	{
		CurrentGold = GameMode->GetGold();		// 현재 골드 초기화
		GameMode->OnGoldChanged.AddLambda(		// 람다식으로 골드 변경 시 현재 골드 저장
			[this](int32 InCurrentGold)
			{
				CurrentGold = InCurrentGold;	// 현재 골드 저장
			}
		);
	}

	// 시작할 때는 안보이게 하기(이 함수가 ATowerBuilder::BeginPlay에서 실행됨)
	Canvas->SetVisibility(ESlateVisibility::Hidden);	// 캔버스 숨기기
	Canvas->SetRenderScale(FVector2D(0.0f, 0.0f));		// Close 테스트 때문에 무조건 0으로 시작
}

void UTowerBuilderWidget::Open()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Open Builder Widget"), *this->GetName());

	// 버튼의 상태를 현재 골드에 맞게 업데이트
	for (UTowerBuildButtonWidget* Button : BuildButtons)
	{
		Button->UpdateButtonState(CurrentGold);
	}	

	Canvas->SetRenderScale(FVector2D(0.0f, 0.0f));		// 크기 초기화
	Canvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);	// 보이게 만들기
	PlayAnimation(OpenAnimation);				// 확대 애니메이션 재생
}

void UTowerBuilderWidget::Close()
{
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Close Builder Widget"), *this->GetName());

	if (Canvas->GetRenderTransform().Scale.X > 0)	// 열려있으면
	{
		//UE_LOG(LogTemp, Warning, TEXT("Close Builder Widget"));
		PlayAnimation(CloseAnimation);	// 닫는 애니메이션 실행
	}
}

void UTowerBuilderWidget::OnCloseAnimationFinished()
{
	// 다 작아지면 안보이게 설정
	Canvas->SetVisibility(ESlateVisibility::Hidden);
	//UE_LOG(LogTemp, Warning, TEXT("Close Animation Finished : %s"), *Canvas->GetRenderTransform().Scale.ToString());
}

void UTowerBuilderWidget::OnBuildButtonClicked(int32 InIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("UTowerBuilderWidget : Mouse Clicked!"));
	//UE_LOG(LogTemp, Warning, TEXT("[%s] : Build Button Clicked : %d"), *this->GetName(), InIndex);

	// 버튼이 클릭되면 닫고
	Close();

	// TowerBuilder에게 InIndex번째 타워를 생성하라고 요청 전달
	OnTowerBuildRequest.ExecuteIfBound(InIndex);
}

