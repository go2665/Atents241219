// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerDefence/Tower/UI/TowerBuilderWidget.h"
#include "HeroTowerWidget.generated.h"


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
	virtual void OnCloseAnimationFinished() override;

private:
	UFUNCTION()
	void OnMainButtonClicked();

	UFUNCTION()
	void OnScreenClicked(AActor* InClickedBuilder);

protected:
	// 영웅타워 건축 시작 버튼
	UPROPERTY(meta = (BindWidget))
	UButton* BuildButton = nullptr;

	// 영웅 타워의 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroTowerWidget")
	TArray<UTowerDataAsset*> HeroTowerDatas;

};
