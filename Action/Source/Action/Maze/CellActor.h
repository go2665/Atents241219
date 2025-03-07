// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDirectionType.h"
#include "CellActor.generated.h"

UCLASS()
class ACTION_API ACellActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACellActor();

	void Initialize(class CellData* InCellData);

	float GetCellHalfSize() const { return CellHalfSize; }

protected:
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnSensorBeginOverlap(
		UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenGate();
	void CloseGate();



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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float CellHalfSize = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cell")
	float GateHalfThickness = 50.0f;

private:
	EDirectionType Path = EDirectionType::None;
	bool bIsClear = false;
	bool bIsOpened = false;
};
