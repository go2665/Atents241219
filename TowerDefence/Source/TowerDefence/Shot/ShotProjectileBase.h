// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Shot/Data/ShotDataAsset.h"
#include "ShotProjectileBase.generated.h"

UCLASS()
class TOWERDEFENCE_API AShotProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnHitEnemy(class AEnemyBase* HitEnemy) {};	// 적과 충돌 시 호출되는 함수

public:	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shot")
	UStaticMeshComponent* Mesh = nullptr;	// 발사체 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
	UShotDataAsset* ShotData = nullptr;	// 발사체 데이터
};
