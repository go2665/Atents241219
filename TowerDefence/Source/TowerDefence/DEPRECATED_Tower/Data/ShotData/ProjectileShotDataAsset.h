// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_DEPRECATED_ShotDataAsset.h"
#include "ProjectileShotDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API UProjectileShotDataAsset : public U_DEPRECATED_ShotDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	UStaticMesh* ProjectileMesh = nullptr; // 총알 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot Data")
	float MoveSpeed = 300.0f; // 발사체 속도(초기 속도)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shot Data")
	TSubclassOf<class AShotProjectileBase> ProjectileClass;	// 발사체 클래스
};
