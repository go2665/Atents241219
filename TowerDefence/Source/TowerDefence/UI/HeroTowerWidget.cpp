// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroTowerWidget.h"
#include "Components/Button.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"

void UHeroTowerWidget::OnSetup()
{
	// 부모 클래스의 초기화 함수 호출
	Super::OnInitialize(&HeroTowerDatas);	

	// 버튼 클릭 시 실행될 함수 바인딩
	BuildButton->OnClicked.AddDynamic(this, &UHeroTowerWidget::OnMainButtonClicked);	

	// 버튼 밖 클릭 시 실행될 함수 바인딩
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());
	if (PlayerController)
	{
		PlayerController->OnMouseClickInput.AddDynamic(this, &UHeroTowerWidget::OnScreenClicked);
	}
}


void UHeroTowerWidget::OnMainButtonClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("Main Build Button Clicked!"));	// 버튼 클릭 시 로그 출력
	Open();
}

void UHeroTowerWidget::OnScreenClicked(AActor* InClickedBuilder)
{
	//UE_LOG(LogTemp, Warning, TEXT("Screen Clicked!"));	// 화면 클릭 시 로그 출력
	Close();	// 화면 클릭 시 위젯 닫기
}
