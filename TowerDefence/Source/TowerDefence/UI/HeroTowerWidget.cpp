// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroTowerWidget.h"
#include "Components/Button.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"
#include "TowerDefence/Tower/Tower.h"

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
		PlayerController->OnMouseClickInput.AddUObject(this, &UHeroTowerWidget::OnScreenClicked);
	}
}

void UHeroTowerWidget::OnBuildButtonClicked(int32 InIndex)
{
	Super::OnBuildButtonClicked(InIndex);	// 부모 클래스의 버튼 클릭 함수 호출
	BuildHeroTower(InIndex);				// 영웅 타워 건축 함수 호출
}


void UHeroTowerWidget::OnMainButtonClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("Main Build Button Clicked!"));	// 버튼 클릭 시 로그 출력
	Open();
}

void UHeroTowerWidget::OnScreenClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("Screen Clicked!"));	// 화면 클릭 시 로그 출력
	Close();	// 화면 클릭 시 위젯 닫기
}

void UHeroTowerWidget::BuildHeroTower(int32 InIndex)
{
	UWorld* World = GetWorld();
	ATowerDefenceGameMode* GameMode = Cast<ATowerDefenceGameMode>(World->GetAuthGameMode());
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());	

	if (GameMode->UseGold(HeroTowerDatas[InIndex]->TowerCost)) // 골드 사용 시도
	{
		FVector SpawnLocation = FVector::ZeroVector;
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult))
		{
			SpawnLocation = HitResult.Location;
		}

		ATower* Tower = World->SpawnActor<ATower>(
			HeroTowerDatas[InIndex]->TowerClass, SpawnLocation, FRotator::ZeroRotator);
		Tower->SetInitialSellCost(HeroTowerDatas[InIndex]->TowerCost * 0.5f);	// 판매 가격 초기화

		Tower->OnTowerSell.AddUObject(GameMode, &ATowerDefenceGameMode::AddGold);	// 타워가 팔렸을 때 골드 추가하도록 함수 연결
		Tower->OnTowerSell.AddLambda(
			[this](int32 _)
			{
				//Tower = nullptr;	// 타워가 팔렸을 때 빌더의 Tower를 nullptr로 초기화
			}
		);

	}
}