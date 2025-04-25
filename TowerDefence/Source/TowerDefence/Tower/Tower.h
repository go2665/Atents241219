// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/Data/CannonDataAsset.h"
#include "TowerDefence/Tower/Data/ShotDataAsset.h"
#include "Tower.generated.h"

class UTowerUpgradeWidget;
class ACannon;
class UWidgetComponent;
class AEnemyBase;
//class UCannonDataAsset;
//class UShotDataAsset;

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

	// 타워가 공격 할 때 호출되는 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerFire(const TArray<AEnemyBase*>& InTargetEnemies);

	// 타워의 공격당 데미지
	inline float GetDamage() const { return Damage; }

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

	// 타워가 공격할 때 발사체를 만들어서 쏘는 함수
	void ShootProjectile(const TArray<AEnemyBase*>& InTargetEnemies);

	// 타워가 공격할 때 히트스캔으로 처리하는 함수
	void ShootHitScan(const TArray<AEnemyBase*>& InTargetEnemies);

	// 실제 라인트레이스를 수행하는 함수(InTarget 공격 대상, OutHitTargets 맞은 적 목록)
	bool LineTraceToTarget(AActor* InTarget, TArray<AEnemyBase*>& OutHitTargets);

	// 타워의 현재 캐논 데이터
	inline const FCannonLevelData& GetCannonLevelData() const
	{
		return CannonData->LevelData[TowerLevel];
	}

	// 타워의 현재 Shot 데이터
	inline const FShotLevelData& GetShotLevelData() const
	{
		return ShotData->LevelData[TowerLevel];
	}

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

	// 대포 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	UCannonDataAsset* CannonData = nullptr;
	
	// 샷 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	UShotDataAsset* ShotData;

	// 컴포넌트는 이팩트만 관리(추가/삭제 등), 컴포넌트는 블루프린트에서 추가(클래스의 기본값으로 넣지 않아야함)
	// 이팩트가 추가/삭제 되었을 때 타워에 델리게이트로 알림 <- 기능 제거
	
	// 완료 --------------------------------------

	// 타워는 모디파이어를 관리(인터페이스를 통해 컴포넌트가 함수를 호출한다.)
	// 타워는 이팩트에 변화가 있을 때 모디파이어 수정하고 타워에 최종값 수정
	// 스텟이 필요한 행위(공격, 스킬)는 타워에서 받아오기

	// 스텟 : 기본값(타워가 데이터 파일에 의해 기본적으로 가지는 값)
	// 모디파이어 : 배율(버프/디버프로 인해 스텟을 증폭시키는 정도)
	// 최종값 : 스텟에 모디파이어가 곱해진 값



	// Effect : 버프 데이터(컴포넌트 추가), 모디파이어
	// 스킬 데이터(컴포넌트 추가)
	// 주변에 버프 제공(컴포넌트 추가?)
	// 스텟
	
	// 타워가 주는 공격당 데미지(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Effect Modified Value")
	float Damage = 1.0f;	

	// 대포 사정거리(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Effect Modified Value")
	float Range = 300.0f;	

	// 발사 속도(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Effect Modified Value")
	float FireRate = 2.0f;	

	// 공격 시 한번에 공격 가능한 타겟 수(모디파이어 적용된 값. 버프 변경시 재계산되어야 함)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Effect Modified Value")
	int32 TargetCount = 1;

	// 디버그 정보 표시 여부
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debug")
	bool bShowDebugInfo = false;	

private:
	// 총기 클래스의 인스턴스
	UPROPERTY()
	ACannon* CannonInstance = nullptr;		

	UPROPERTY()
	UTowerUpgradeWidget* UpgradeWidgetInstance = nullptr;	// 업그레이드 위젯 인스턴스

	// 대포 레벨의 최대 + 1
	int8 TowerLevelCap = 3;	

	// 타워 판매 비용
	int32 SellCost = 50;	

};
