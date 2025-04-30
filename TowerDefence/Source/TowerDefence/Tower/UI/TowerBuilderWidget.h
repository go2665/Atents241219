// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/Data/TowerDataAsset.h"
#include "TowerBuildButtonWidget.h"
#include "TowerBuilderWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnTowerBuildRequest, int32);

//class UTowerBuildButtonWidget;
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

private:
	// 이 위젯이 가지고 있는 버튼들이 클릭 시 호출되는 함수
	UFUNCTION()
	void OnBuildButtonClicked(int32 InIndex);

public:
	// 타워 건설 요청을 보내는 델리게이트
	FOnTowerBuildRequest OnTowerBuildRequest;

protected:
	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Canvas = nullptr;

	// 버튼의 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TowerBuilder")
	TSubclassOf<UTowerBuildButtonWidget> TowerButtonButtonWidgetClass;

private:
	// 타워 데이터 배열의 주소(초기화 때 전달받음)
	const TArray<UTowerDataAsset*>* TowerDatas = nullptr;

};
