// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/Data/TowerDataAsset.h"
#include "TowerBuildButtonWidget.h"
#include "TowerBuilderWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnTowerBuildRequest, int32);

class UCanvasPanel;
class USizeBox;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UTowerBuilderWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 타워 빌더 위젯 초기화. TowerBuilder가 실행
	void OnInitialize(const TArray<UTowerDataAsset*>* InTowerDatas);

	// 위젯을 여는 함수
	void Open();

	// 위젯을 닫는 함수
	void Close();

protected:
	// 닫는 애니메이션이 끝난 후 실행될 함수
	UFUNCTION()
	virtual void OnCloseAnimationFinished();
	
	// 이 위젯이 가지고 있는 버튼들이 클릭 시 호출되는 함수
	UFUNCTION()
	virtual void OnBuildButtonClicked(int32 InIndex);

public:
	// 타워 건설 요청을 보내는 델리게이트
	FOnTowerBuildRequest OnTowerBuildRequest;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* OpenAnimation = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* CloseAnimation = nullptr;

	// 버튼의 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TowerBuilder")
	TSubclassOf<UTowerBuildButtonWidget> TowerButtonButtonWidgetClass;

	// 버튼 인스턴스 배열
	UPROPERTY()
	TArray<UTowerBuildButtonWidget*> BuildButtons;	

	// 현재 골드(게임 모드에서 가져와야 함)
	int32 CurrentGold = 0;	
};
