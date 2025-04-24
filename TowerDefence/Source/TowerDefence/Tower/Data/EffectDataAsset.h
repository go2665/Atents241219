// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectDataAsset.generated.h"

class UEffectBase;
/**
 * 
 */
UCLASS(Blueprintable)
class TOWERDEFENCE_API UEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 이팩트 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	FText BuffName;	

	// 이팩트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	TSubclassOf<UEffectBase> EffectClass;	

	// 이팩트 기본 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	float Duration = 1.0f; 

	// 이팩트 기본 스택 회수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	int32 MaxStackCount = 1; 

	// 이팩트 수치 1(변화 비율. 예) 0.5면 50% 증가, -0.3이면 30% 감소)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	float Modifier1 = 0.0f;	

	// 이팩트 수치 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	float Modifier2 = 0.0f;	

	// 이팩트 수치 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Data")
	float Modifier3 = 0.0f;	
};
