// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

class UTowerUpgradeWidget;
class ACannonBase;
class UCannonDataAsset;
class UWidgetComponent;

/*
타워 클래스. 데이터 파일을 기반으로 기능이 변화함.
*/
UCLASS()
class TOWERDEFENCE_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	ATower();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:	
	// 총기 레벨업 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerLevelUp(); 

	// 타워 판매 함수
	void TowerSell(); 

	//// 버프 추가 함수
	//bool AddEffect(UTowerBuffDataAsset* BuffData);
	
	// 버프 제거 함수
	//bool RemoveEffect(UTowerBuffDataAsset* BuffData);

private:
	// 업그레이드 UI 닫기 or 스킬 사용에 사용됨(DEPRECATE된 OnCancelClicked)
	// InClickedTower : 클릭한 타워(nullptr일 수 있다.)
	UFUNCTION()
	void OnScreenClicked(AActor* InClickedTower);

	// 버프 모디파이어 재계산
	void RefreshModifiers();

protected:
	// 타워 메시 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TowerBodyMesh = nullptr;

	// 업그레이드 UI 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* UpgradeWidget = nullptr;

	// 대포 레벨(0~2, 표시할 때는 +1)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data", meta = (ClampMin = "0", ClampMax = "2"))
	int32 CannonLevel = 0; 

	// 타워 설치 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	int32 TowerCost = 100; 	

	// 대포 데이터(레벨별 1개씩)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	TArray<UCannonDataAsset*> CannonDatas;
	
	// 샷 데이터
	// 버프 데이터(컴포넌트 추가)
	// 스킬 데이터(컴포넌트 추가)

private:
	// 총기 클래스의 인스턴스
	UPROPERTY()
	ACannonBase* CannonInstance = nullptr;		

	UPROPERTY()
	UTowerUpgradeWidget* UpgradeWidgetInstance = nullptr;	// 업그레이드 위젯 인스턴스

	// 대포 레벨의 최대값
	const static int8 MaxCannonLevel = 2;	

	// 타워 판매 비용
	int32 SellCost = 50;	
};
