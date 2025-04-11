// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShotDataAsset.h"
#include "ProjectileShotDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UProjectileShotDataAsset : public UShotDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	UStaticMesh* ProjectileMesh = nullptr; // 총알 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float MoveSpeed = 300.0f; // 발사체 속도(초기 속도)
};
