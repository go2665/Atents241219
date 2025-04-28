// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Data/GunData/GunDataAsset.h"
#include "GunBaseActor.generated.h"

class ATowerBaseActor;

UCLASS()
class TOWERDEFENCE_API AGunBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	// 총기 초기화 함수
	void OnInitialize(ATowerBaseActor* InOwnerTower);

	// Gun Level 설정
	void SetGunLevel(int Level);

	// 버프 모디파이어 재적용
	void RefreshBuffModifiers();

	// 현재 업그레이드 비용 반환
	inline int GetCurrentUpgradeCost() const { return CurrentGunData->UpgradeCost; }

	// 부모 타워 설정

protected:
	virtual void Shoot();

	// 디버프에 적용할 모디파이어 값 반환받는 함수
	float GetModifierForDebuff(EDebuffType InType);

private:
	UFUNCTION()
	void OnSightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PrintEnemyList();

	void ShootStart(float InFirstDelay = -1.0f);
	void ShootStop();

	void LookFirstTarget(float DeltaTime);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GunMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SightSensor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* MuzzleLocation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun", meta = (ClampMin = "0.1"))
	float ScaleFactor = 2.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TArray<UGunDataAsset*> GunDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	float FireFirstDelay = 0.5f;	// 첫 발사 지연 시간

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	float LookSpeed = 360.0f;		// 적을 바라보는 속도(FireFirstDelay초 안에 180도 이상 회전 할 수 있는 속도이어야 한다.)
		
	UGunDataAsset* CurrentGunData = nullptr;

	// 적의 목록 : TArray
	TArray<class AEnemyBase*> TargetEnemies;

	// 이 Gun이 배치된 타워
	UPROPERTY()
	ATowerBaseActor* OwnerTower = nullptr;	

private:
	FTimerHandle ShootTimerHandle;	// 발사 타이머 핸들

	bool bIsShootingReset = false;
	
};
