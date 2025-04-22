// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/Data/CannonDataAsset.h"
#include "Tower.generated.h"

class UTowerUpgradeWidget;
class ACannon;
//class UCannonDataAsset;
class UWidgetComponent;
class AEnemyBase;
class UShotDataAsset;

/*
타워 클래스. 데이터 파일을 기반으로 기능이 변화함.
*/
UCLASS()
class TOWERDEFENCE_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	ATower();

protected:
	virtual void BeginPlay() override;

public:	
	// 총기 레벨업 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerLevelUp(); 

	// 타워 판매 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerSell(); 

	//// 버프 추가 함수
	//bool AddEffect(UTowerBuffDataAsset* BuffData);
	
	// 버프 제거 함수
	//bool RemoveEffect(UTowerBuffDataAsset* BuffData);

	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerFire(const TArray<AEnemyBase*>& InTargetEnemies);

	// 타워의 현재 캐논 데이터
	inline const UCannonDataAsset* GetCannonData() const
	{
		if (TowerLevel < CannonDatas.Num())
		{
			return CannonDatas[TowerLevel];
		}
		return nullptr;
	}

	// 타워의 사정거리
	inline float GetRange() const { return Range; }

	// 타워의 발사 속도
	inline float GetFireRate() const { return FireRate; }

	// 타워의 공격 시 한번에 공격 가능한 타겟 수
	inline float GetTargetCount() const { return TargetCount; }

private:
	// 타워 클릭했을 때 실행(타워 업그레이드 UI 위젯 열기에 사용됨)
	UFUNCTION()
	void OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed);

	// 업그레이드 UI 닫기 or 스킬 사용에 사용됨(DEPRECATE된 OnCancelClicked)
	// InClickedTower : 클릭한 타워(nullptr일 수 있다.)
	UFUNCTION()
	void OnScreenClicked(AActor* InClickedTower);

	// 버프 모디파이어 재계산
	void UpdateModifiers();

	// 타워 데이터를 전체 갱신하는 함수
	void UpdateData();

	// 공격하는 적 목록 출력하기
	void Test_PrintFireTargetList(const TArray<AEnemyBase*>& InTargetEnemies);

protected:
	// 타워 메시 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TowerBodyMesh = nullptr;

	// 업그레이드 UI 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* UpgradeWidget = nullptr;

	// 타워 레벨(0~2, 표시할 때는 +1)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data", meta = (ClampMin = "0", ClampMax = "2"))
	int32 TowerLevel = 0; 

	// 타워 설치 비용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	int32 TowerCost = 100; 	

	// 대포 데이터(레벨별 1개씩)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	TArray<UCannonDataAsset*> CannonDatas;
	
	// 샷 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	TArray<UShotDataAsset*> ShotDatas;

	// 버프 데이터(컴포넌트 추가)
	// 스킬 데이터(컴포넌트 추가)
	// 스텟
	// 모디파이어
	
	// 대포 사정거리(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Buff Modified Value")
	float Range = 300.0f;	

	// 발사 속도(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Buff Modified Value")
	float FireRate = 2.0f;	

	// 공격 시 한번에 공격 가능한 타겟 수(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Buff Modified Value")
	int32 TargetCount = 1;

private:
	// 총기 클래스의 인스턴스
	UPROPERTY()
	ACannon* CannonInstance = nullptr;		

	UPROPERTY()
	UTowerUpgradeWidget* UpgradeWidgetInstance = nullptr;	// 업그레이드 위젯 인스턴스

	// 대포 레벨의 최대값
	const static int8 MaxTowerLevel = 2;	

	// 타워 판매 비용
	int32 SellCost = 50;	
};
