// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Gun/Data/GunDataAsset.h"
#include "GunBaseActor.generated.h"

UCLASS()
class TOWERDEFENCE_API AGunBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Gun Level 설정
	void SetGunLevel(int Level);

private:
	UFUNCTION()
	void OnSightOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSightOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void PrintEnemyList();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GunMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SightSensor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun", meta = (ClampMin = "0.1"))
	float ScaleFactor = 2.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun")
	TArray<UGunDataAsset*> GunDatas;
		
	UGunDataAsset* CurrentGunData = nullptr;

	// 적의 목록 : TArray
	TArray<class AEnemyBase*> TargetEnemies;
	
};
