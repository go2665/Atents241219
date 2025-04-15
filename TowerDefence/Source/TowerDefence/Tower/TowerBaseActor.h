// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Gun/GunBaseActor.h"
#include "TowerBaseActor.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBaseActor();
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void LevelUp(); // 총기 레벨업 함수

	void Sell(); // 타워 판매 함수

	// 현재 업그레이드 비용 반환
	inline int32 GetCurrentUpgradeCost() const { return Gun->GetCurrentUpgradeCost(); }; 

private:
	UFUNCTION()
	void OnTowerClicked(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION()
	void OnCancelClicked(AActor* InClickedTower);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UChildActorComponent* GunActor = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TowerBodyMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* UpgradeWidget = nullptr;

	// 일반 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	TSubclassOf<AGunBaseActor> GunClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower", meta = (ClampMin = "1", ClampMax = "3"))
	int32 GunLevel = 1; // 총기 레벨(1~3)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	int32 TowerCost = 100; // 타워 설치 비용

private:
	const static int8 MaxGunLevel = 3;	// 총기 레벨의 최대값

	UPROPERTY()
	AGunBaseActor* Gun = nullptr;		// 총기 클래스의 인스턴스

	UPROPERTY()
	class UTowerUpgradeWidget* UpgradeWidgetInstance = nullptr;	// 업그레이드 위젯 인스턴스

	int32 SellCost = 0;	// 타워 판매 비용
};
