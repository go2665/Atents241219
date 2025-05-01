// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Components/Buffer/BufferDataAsset.h"
#include "BufferComponent.generated.h"

class ATower;			// 전방 선언(타워)
class ATowerBuilder;	// 전방 선언(타워 배치 가능한 액터)
//class UBufferDataAsset;	// 버퍼 데이터 파일

/*
* 주변에 있는 타워에 버프를 거는 컴포넌트
*/
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBufferComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Buffer|Test")
	void Test_ChangeBufferData(UBufferDataAsset* InData) {
		BufferDataAsset = InData; 
		ResetBuffTimer();
	};

private:
	// 주변 타워에 버프를 건다(타워 배치 가능한 액터를 순회하면서 버프를 건다)
	UFUNCTION()
	void AddBuffToAround();

	// 버프를 거는 타이머를 정리하고 다시 시작하는 함수
	void ResetBuffTimer();

	// 현재 레벨의 데이터 참조 가져오기
	inline const FBufferLevelData& GetLevelData() const { return BufferDataAsset->LevelData[Level]; }	

	// 현재 레벨의 버프 타입
	inline EEffectType GetBuffType() const { return BufferDataAsset->EffectType; }

	// 현재 레벨의 인터벌
	inline float GetInterval() const { return GetLevelData().Interval; }

	// 현재 레벨의 반지름
	inline float GetRadius() const { return GetLevelData().Radius; }	

protected:
	// 버퍼 데이터 파일(레벨별 버프 타입, 인터벌, 반지름)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffer")
	UBufferDataAsset* BufferDataAsset = nullptr;
	
	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffer")
	int32 Level = 0; // (0에서 시작)

	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilder)를 저장하는 배열(시작하면 변하지 않는다)
	UPROPERTY(VisibleAnywhere, Category = "Buffer")
	TArray<ATowerBuilder*> TowerBuilderList;

private:
	// 버프 타이머 핸들
	FTimerHandle BufferTimerHandle;

	UPROPERTY()
	ATower* Tower = nullptr;	// 소유자(타워) 가져오기

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bShowDebugInfo = false;
};
