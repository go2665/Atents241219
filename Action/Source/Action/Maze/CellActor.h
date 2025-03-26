// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDirectionType.h"
#include "CellActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCellClear, ACellActor*, InClearCell);

UCLASS()
class ACTION_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellActor();

	// 셀 데이터로 초기화 작업
	void Initialize(class CellData* InCellData);

	float GetCellHalfSize() const { return CellHalfSize; }
	void SetClear() { bIsClear = true; }

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;
#if WITH_EDITOR	// 에디터에서만 포함되게 전처리기 지시문 사용(WITH_EDITOR가 선언되어 있을 때만 컴파일 때 포함)
	// 프로퍼티 변경 시 호출
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// 셀 내부에 플레이어가 들어왔을 때 호출
	UFUNCTION()
	void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 적이 죽었을 때 호출
	UFUNCTION()
	void OnEnemyDie();

	// 벽이 아닌 문을 여는 함수
	void OpenGate();

	// 모든 문을 닫는 함수
	void CloseGate();

	// 적을 스폰하는 함수
	void SpawnEnemy();

public:
	FOnCellClear OnCellClear;

protected:
	// 배경 매시(벽과 바닥)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell")
	class UStaticMeshComponent* BackgroundMesh = nullptr;

	// 문 매시의 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell")
	TArray<class UStaticMeshComponent*> GateMeshArray;

	// 셀 안에 플레이어가 들어왔다는 것을 감지하기 위한 콜리젼
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cell")
	class UBoxComponent* SensorCollision = nullptr;

	// 셀 크기의 절반
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float CellHalfSize = 1000.0f;

	// 문의 두께의 절반
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float GateHalfThickness = 50.0f;

	// 생성할 적 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cell")
	TSubclassOf<class AEnemyBase> EnemyClass = nullptr;

	// 생성할 적의 최소수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cell", meta = (ClampMin = "1"))
	int32 EnemyCountMin = 1;

	// 생성할 적의 최대수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cell", meta = (ClampMin = "1"))
	int32 EnemyCountMax = 5;
	
private:
	// 셀의 게이트 중 벽이 아닌 방향을 기록해 놓은 비트	플래그
	EDirectionType Path = EDirectionType::None;

	// 셀이 클리어 상태인지 여부
	bool bIsClear = false;

	// 셀이 열려 있는지 여부
	bool bIsOpened = false;

	// 현재 적이 생성되어 있는 수
	int32 SpawnCount = 0;
};
