// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "BufferComponent.generated.h"

class ATowerBuilder;	// 전방 선언(타워 배치 가능한 액터)
class UBufferDataAsset;	// 버퍼 데이터 파일

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

private:
	// 주변 타워에 버프를 건다(타워 배치 가능한 액터를 순회하면서 버프를 건다)
	UFUNCTION()
	void AddBuffToAround();

	inline EEffectType GetBuffType() const { return EEffectType::None; }	// 데이터 파일에서 읽기
	inline float GetInterval() const { return 0.5f; }	// 데이터 파일에서 읽기
	inline float GetRadius() const { return 100.0f; }	// 데이터 파일에서 읽기
		
protected:
	// 버퍼 데이터 파일(레벨별 버프 타입, 인터벌, 반지름)
	UBufferDataAsset* BufferDataAsset = nullptr;
	
	// 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffer")
	int32 Level = 0; // (0에서 시작)

	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 저장하는 배열(시작하면 변하지 않는다)
	UPROPERTY(VisibleAnywhere, Category = "Tower|Buff")
	TArray<ATowerBuilder*> TowerBuilderList;

private:
	// 버프 타이머 핸들
	FTimerHandle BuffTimerHandle;
};
