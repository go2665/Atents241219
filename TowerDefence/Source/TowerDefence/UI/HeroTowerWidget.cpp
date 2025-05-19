// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroTowerWidget.h"
#include "Components/Button.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "TowerDefence/Framework/TowerDefenceGameMode.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Player/PlayerSpectatorPawn.h"
#include "TowerDefence/Components/Skill/SkillComponent.h"

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

		// 영웅 타워 건축 완료 시 실행될 함수 바인딩
		APlayerSpectatorPawn* PlayerPawn = Cast<APlayerSpectatorPawn>(PlayerController->GetPawn());
		if (PlayerPawn)
		{
			PlayerPawn->OnHeroTowerBuildComplete.BindUObject(this, &UHeroTowerWidget::OnHeroTowerBuildComplete);
		}
	}

	// 버튼의 머티리얼 가져오기
	const FSlateBrush* OriginalBrush = &(BuildButton->GetStyle().Normal);
	UMaterialInterface* OriginalMaterial = Cast<UMaterialInterface>(OriginalBrush->GetResourceObject());
	ButtonMID = UMaterialInstanceDynamic::Create(OriginalMaterial, this, TEXT("TestMID"));
	ButtonMID->SetTextureParameterValue(TEXT("ImageTexture"), BackgroudImage);

	// MID 적용한 새 브러시 설정
	FButtonStyle ButtonStyle = BuildButton->GetStyle();
	FSlateBrush ImageBrush = *OriginalBrush;
	ImageBrush.SetResourceObject(ButtonMID);
	ButtonStyle.SetNormal(ImageBrush);
	ButtonStyle.SetHovered(ImageBrush);
	ButtonStyle.SetPressed(ImageBrush);
	ButtonStyle.SetDisabled(ImageBrush);
	BuildButton->SetStyle(ButtonStyle);

	// 게임모드 찾아놓기
	GameMode = Cast<ATowerDefenceGameMode>(World->GetAuthGameMode());
}

void UHeroTowerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	// 부모 클래스의 틱 함수 호출

	if (TowerSkillComponent)
	{
		float value = FMath::Max(0.0f, TowerSkillComponent->GetCoolTimeRatio());
		ButtonMID->SetScalarParameterValue(TEXT("Ratio"), value);
	}
}

void UHeroTowerWidget::OnBuildButtonClicked(int32 InIndex)
{
	Super::OnBuildButtonClicked(InIndex);	// 부모 클래스의 버튼 클릭 함수 호출
	BuildHeroTower(InIndex);				// 영웅 타워 건축 함수 호출
}

void UHeroTowerWidget::OnMainButtonClicked()  
{  
	//UE_LOG(LogTemp, Warning, TEXT("UHeroTowerWidget::OnMainButtonClicked : Mouse Clicked!"));
	//UE_LOG(LogTemp, Warning, TEXT("Main Build Button Clicked! : [%d]"), static_cast<int32>(State)); // 버튼 클릭 시 로그 출력  

	switch (State)  
	{  
	case UHeroTowerWidget::EBuilderState::Ready:  
		if (GameMode->IsAnyEmptyTowerBuilder())
		{
			Open();  
		}
		break;  
	case UHeroTowerWidget::EBuilderState::Build:  
		// 스킬 사용 모드로 진입  
		if (TowerSkillComponent)  
		{  
			TowerSkillComponent->ActivateSkill(); // 스킬 활성화  
		}  
		break;  
	default:  
		break;  
	}  
}

void UHeroTowerWidget::OnScreenClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("UHeroTowerWidget::OnScreenClicked : Mouse Clicked!"));
	//UE_LOG(LogTemp, Warning, TEXT("Screen Clicked!"));	// 화면 클릭 시 로그 출력
	Close();	// 화면 클릭 시 위젯 닫기	
}

void UHeroTowerWidget::OnHeroTowerBuildComplete(ATower* InTower)
{
	if (InTower)
	{
		// 영웅타워가 건설 완료되었다.
		UE_LOG(LogTemp, Warning, TEXT("Hero Tower Build Complete!"));
		const UTowerDataAsset* TowerData = InTower->GetTowerData();
		// 버튼의 머티리얼 인스턴스에 타워 데이터 설정
		ButtonMID->SetTextureParameterValue(TEXT("ImageTexture"), TowerData->TowerImage);
		ButtonMID->SetScalarParameterValue(TEXT("Ratio"), 1.0f);

		// 타워의 스킬 컴포넌트 가져오기
		TowerSkillComponent = InTower->FindComponentByClass<USkillComponent>();
	}
	else
	{
		// 영웅타워가 판매되었다.
		UE_LOG(LogTemp, Warning, TEXT("Hero Tower Sold!"));
		ButtonMID->SetTextureParameterValue(TEXT("ImageTexture"), BackgroudImage);
		ButtonMID->SetScalarParameterValue(TEXT("Ratio"), 0.0f);
		State = EBuilderState::Ready;
		TowerSkillComponent = nullptr;
	}
}

void UHeroTowerWidget::BuildHeroTower(int32 InIndex)
{
	UWorld* World = GetWorld();
	ATowerDefencePlayerController* PlayerController =
		Cast<ATowerDefencePlayerController>(World->GetFirstPlayerController());	
	APlayerSpectatorPawn* PlayerPawn = Cast<APlayerSpectatorPawn>(PlayerController->GetPawn());

	if (GameMode && PlayerController && PlayerPawn)
	{
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
			Tower->SetInitialTowerData(HeroTowerDatas[InIndex]);	// 타워 데이터 초기화
			Tower->OnTowerSell.AddUObject(GameMode, &ATowerDefenceGameMode::AddGold);	// 타워가 팔렸을 때 골드 추가하도록 함수 연결
			Tower->OnTowerSell.AddLambda(
				[this](float _)
				{
					this->OnHeroTowerBuildComplete(nullptr);	// 영웅 타워가 팔렸을 때 델리게이트 호출
				}
			);

			PlayerPawn->SetTemporaryHero(Tower);	// 플레이어의 임시 영웅 타워 설정

			State = EBuilderState::Build;
		}
	}
}