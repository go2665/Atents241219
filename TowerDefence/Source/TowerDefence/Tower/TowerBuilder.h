// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/Data/TowerDataAsset.h"
#include "TowerBuilder.generated.h"

class UTowerBuilderWidget;

UCLASS()
class TOWERDEFENCE_API ATowerBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBuilder();
	inline ATower* GetTower() const { return Tower; }

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "TowerBuilder")
	void Test_BuildTower(int32 TowerIndex);

private:
	// 타워 클릭했을 때 실행(타워 업그레이드 UI 위젯 열기에 사용됨)
	UFUNCTION()
	void OnBuilderClicked(AActor* TouchedActor, FKey ButtonPressed);

	// InClickedTower : 클릭한 타워(nullptr일 수 있다.)
	UFUNCTION()
	void OnScreenClicked(AActor* InClickedBuilder);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent = nullptr;

	// 업그레이드 UI 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* TowerBuildWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBuilder")
	TArray<UTowerDataAsset*> TowerDatas;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TowerBuilder", meta = (AllowPrivateAccess = "true"))
	ATower* Tower = nullptr;

	// 타워 빌더 위젯 인스턴스
	UPROPERTY()
	UTowerBuilderWidget* TowerBuilderWidgetInstance = nullptr;	
};
