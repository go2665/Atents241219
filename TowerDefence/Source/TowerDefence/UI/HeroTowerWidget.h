// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerDefence/Tower/UI/TowerBuilderWidget.h"
#include "HeroTowerWidget.generated.h"

class USkillComponent;
class ATowerDefenceGameMode;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UHeroTowerWidget : public UTowerBuilderWidget
{
	GENERATED_BODY()
	
public:
	// 영웅타워 위젯 초기화.
	void OnSetup();		

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void OnBuildButtonClicked(int32 InIndex) override;

public:
	// 영웅타워 위젯의 배경 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroTowerWidget")
	UTexture* BackgroudImage = nullptr;

private:
	UFUNCTION()
	void OnMainButtonClicked();

	UFUNCTION()
	void OnScreenClicked();

	// 영웅타워가 건설 완료되었을 때 실행되는 함수(영웅타워가 판매되었을 때는 파라메터가 null)
	UFUNCTION()
	void OnHeroTowerBuildComplete(ATower* InTower);

	void BuildHeroTower(int32 InIndex);

protected:
	// 영웅타워 건축 시작 버튼
	UPROPERTY(meta = (BindWidget))
	UButton* BuildButton = nullptr;

	// 영웅 타워의 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroTowerWidget")
	TArray<UTowerDataAsset*> HeroTowerDatas;

private:
	// 버튼의 머티리얼 인스턴스
	UPROPERTY()
	UMaterialInstanceDynamic* ButtonMID = nullptr;	

	// 버튼이 건설한 영웅타워의 스킬 컴포넌트
	USkillComponent* TowerSkillComponent = nullptr;

	// 게임모드
	ATowerDefenceGameMode* GameMode = nullptr;

	enum class EBuilderState
	{
		NoPlace,
		Ready,
		Build
	};

	// 이 UI를 사용할 수 있는지 여부
	EBuilderState State = EBuilderState::Ready;

};
