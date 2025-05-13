// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/Data/TowerDataAsset.h"
#include "TowerDefence/Tower/Data/CannonDataAsset.h"
#include "TowerDefence/Tower/Data/ShotDataAsset.h"
#include "TowerDefence/Components/Effect/EffectTargetable.h"
#include "Tower.generated.h"

class UTowerUpgradeWidget;
class ACannon;
class UWidgetComponent;
class AEnemy;
class UEffectComponent;
//class UCannonDataAsset;
//class UShotDataAsset;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTowerLevelUp, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTowerSell, int32);

/*
타워 클래스. 데이터 파일을 기반으로 기능이 변화함.
*/
UCLASS()
class TOWERDEFENCE_API ATower : public AActor, public IEffectTargetable
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

	// 타워가 공격 할 때 호출되는 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void TowerFire(const TArray<AEnemy*>& InTargetEnemies);

	// 버프 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	bool AddEffect(EEffectType InType, int32 InLevel) override;
	
	// 버프 제거 함수
	UFUNCTION(BlueprintCallable, Category = "Tower")
	bool RemoveEffect(EEffectType InType) override;

	// 받아온 모디파이어를 자신에게 적용해서 최종 값을 만드는 함수
	void ApplyModifiers(const TMap<EEffectModifier, float>* InModifierMap) override;

	// 모디파이어 맵을 가져오는 함수
	inline const TMap<EEffectModifier, float>* GetModifiersMap() const override { return EffectModifiers; }	

	// 특정 모디파이어 값을 가져오는 함수
	inline float GetModifier(EEffectModifier ModifierType) const override
	{
		if (EffectModifiers && EffectModifiers->Contains(ModifierType))
		{
			return (*EffectModifiers)[ModifierType];
		}
		return 0.0f;
	}
	// 이팩트를 받는 아군인지 적군인지 알려주는 함수
	inline EEffectTarget GetEffectTarget() const override { return EEffectTarget::Friendly; }

	// 이팩트의 특수효과가 보일 위치를 가져오는 함수
	inline FVector GetEffectLocation() const override 
	{
		if (TowerBodyMesh)
		{
			return TowerBodyMesh->GetSocketLocation(FName("TowerRoof"));
		}
		else
		{
			return GetActorLocation();
		}
	};

	// 타워의 공격당 데미지
	inline float GetDamage() const { return Damage; }
	
	// 타워의 사정거리
	inline float GetRange() const { return Range; }

	// 타워의 발사 속도
	inline float GetFireRate() const { return FireRate; }

	// 타워의 공격 시 한번에 공격 가능한 타겟 수
	inline float GetTargetCount() const { return TargetCount; }

	// 타워 데이터 에셋을 읽기 전용으로 가져오는 함수
	inline const UTowerDataAsset* GetTowerData() const { return TowerData; }

	// 타워 데이터 에셋을 설정하고 초기화 하는 함수
	inline void SetInitialTowerData(UTowerDataAsset* InTowerData)
	{
		TowerData = InTowerData;
		if (TowerData)
		{
			SellCost = TowerData->TowerCost * 0.5f; // 판매 가격 초기화
		}
	}

	inline bool GetShowDebugInfo() const { return bShowDebugInfo; }

	inline void ActivateTower()	{ bIsActivated = true; }

protected:
	// 업그레이드 UI 닫기 or 스킬 사용에 사용됨(DEPRECATE된 OnCancelClicked)
	UFUNCTION()
	virtual void OnScreenClicked();

private:
	// 타워 클릭했을 때 실행(타워 업그레이드 UI 위젯 열기에 사용됨)
	UFUNCTION()
	void OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed);
	
	// 타워 데이터를 전체 갱신하는 함수
	void UpdateData();

	// 타워가 공격할 때 발사체를 만들어서 쏘는 함수
	void ShootProjectile(const TArray<AEnemy*>& InTargetEnemies);

	// 타워가 공격할 때 히트스캔으로 처리하는 함수
	void ShootHitScan(const TArray<AEnemy*>& InTargetEnemies);

	// 실제 라인트레이스를 수행하는 함수(InTarget 공격 대상, OutHitTargets 맞은 적 목록)
	bool LineTraceToTarget(AActor* InTarget, TArray<AEnemy*>& OutHitTargets);

	// 타워의 현재 캐논 데이터
	inline const FCannonLevelData& GetCannonLevelData() const
	{
		return CannonData->LevelData[TowerLevel];
	}

	// 타워의 현재 Shot 데이터(기본값)
	inline const FShotLevelData& GetShotLevelData() const
	{
		return ShotData->LevelData[TowerLevel];
	}
	
	// 공격하는 적 목록 출력하기
	void Test_PrintFireTargetList(const TArray<AEnemy*>& InTargetEnemies);

	//// 타워의 체력 설정(타워는 체력이 없음. 사용될 일이 없어야 한다.)
	//inline virtual void SetHealth(float InHealth) override {};

	//// 타워의 생존 여부(무조건 살아있다. 사용될 일이 없어야 한다.)
	//inline virtual bool IsAlive() const override { return true; };

public:
	// 타워가 레벨업을 했을 때 방송하는 델리게이트(파라메터는 레벨업 한 레벨)
	FOnTowerLevelUp OnTowerLevelUp;

	// 타워가 판매 되었을 때 방송하는 델리게이트(파라메터는 판매한 금액)
	FOnTowerSell OnTowerSell;	

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
	
	// 타워 데이터(읽기 전용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	UTowerDataAsset* TowerData = nullptr;

	// 대포 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	UCannonDataAsset* CannonData = nullptr;
	
	// 샷 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	UShotDataAsset* ShotData;

	// 타워 판매 비용(판매 비용은 (설치 비용 + 업그레이드 비용)의 절반)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	int32 SellCost = 0;

	// 타워가 활성화 되었는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower|Base Data")
	bool bIsActivated = false;	

	// 스킬 데이터(컴포넌트 추가)
	
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

	// 이팩트 컴포넌트 인스턴스
	UPROPERTY()
	UEffectComponent* EffectComponent = nullptr;

	// 모디파이어 맵의 주소(EffectComponent가 전달해준다)
	const TMap<EEffectModifier, float>* EffectModifiers = nullptr;	


};
