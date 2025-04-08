// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShotEnums.h"
#include "ShotDataAsset.generated.h"

/**
 * 총이 발사하는 총알의 데이터
 */
UCLASS()
class TOWERDEFENCE_API UShotDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float Damage = 1;			// 한발 당 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	EDebuffType DebuffType = EDebuffType::None; // 디버프 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	bool bIsAreaAttack = false;					// 단일/범위 공격 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	EAttributeType AttributeType = EAttributeType::None; // 속성 타입
};
