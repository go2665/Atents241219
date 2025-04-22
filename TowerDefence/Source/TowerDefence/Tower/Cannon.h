// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "Cannon.generated.h"

class ATower;
class USphereComponent;
class USceneComponent;
class AEnemyBase;

// 발사 시 타겟 적들을 전달하는 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCannonFire, const TArray<AEnemyBase*>&, TargetEnemies); 

/*
대포 클래스. 범위 안에 있는 적 관리 및 타게팅, 발사 신호를 담당
*/
UCLASS()
class TOWERDEFENCE_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();
	virtual void Tick(float DeltaTime) override;

public:
	// 대포 초기화 함수
	void OnInitialize(ATower* InTower, EFireOrder InOrder = EFireOrder::Discovery);

	// 타워의 모디파이어가 변경되었을 때 호출되는 함수(타워가 호출하는 함수)
	void OnModifierChange();

	// 포탄 발사 위치
	inline FVector GetMuzzleLocation() const { return MuzzleLocation->GetComponentLocation(); }

	// 포탄 발사 방향
	inline FVector GetMuzzleForwardVector() const { return MuzzleLocation->GetForwardVector(); }

private:
	// 시야 범위안에 들어온 적을 관리하는 함수
	UFUNCTION()
	void OnSightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 시야 범위에서 나간 적을 관리하는 함수
	UFUNCTION()
	void OnSightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 발사 시작(주기적으로 Fire() 호출)
	void FireStart(float InFirstDelay = -1.0f);

	// 실제 한발 발사라하고 신호보내는 함수
	void Fire();

	// 발사 중지(타이머 정지)
	void FireStop();

	// 대포가 첫번째 타겟을 바라보도록 회전
	void LookFirstTarget(float DeltaTime);

	// 테스트용 : 적 목록을 출력하는 함수
	void Test_PrintEnemyList();

public:
	// Fire()에서 타겟 적들을 전달하는 델리게이트
	FOnCannonFire OnCannonFire; 


protected:
	// 대포 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CannonMesh = nullptr;

	// 시야 감지용 구형 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SightSensor = nullptr;

	// 발사 트랜스폼 표시용 씬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* MuzzleLocation = nullptr;

	// 메시 스케일 조정용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Cannon", meta = (ClampMin = "0.1"))
	float ScaleFactor = 2.0f;

	// 첫 발사 지연 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Cannon")
	float FireFirstDelay = 0.5f;

	// 적을 바라보는 속도(FireFirstDelay초 안에 180도 이상 회전 할 수 있는 속도이어야 한다.)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Cannon")
	float LookSpeed = 360.0f;

private:
	// 부모 타워
	UPROPERTY()
	ATower* ParentTower = nullptr;

	// 적의 목록 : TArray
	TArray<AEnemyBase*> TargetEnemies;

	// 발사 타이머 핸들
	FTimerHandle ShootTimerHandle;	

	// 적을 공격하는 순서(기본값 : 발견 순서)
	EFireOrder FireOrder = EFireOrder::Discovery;

	// 발사 속도 재설정 플래그(다음 발사 시점에 발사 속도 재적용)
	bool bShootingResetFlag = false;
};
