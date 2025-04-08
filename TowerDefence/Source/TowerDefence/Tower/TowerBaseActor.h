// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBaseActor.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Tower")
	void LevelUp(); // 총기 레벨업 함수


protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UChildActorComponent* GunActor = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TowerBodyMesh = nullptr;

	// 일반 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	TSubclassOf<class AGunBaseActor> GunClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower", meta = (ClampMin = "1", ClampMax = "3"))
	int32 GunLevel = 1; // 총기 레벨(1~3)

private:
	const static int8 MaxGunLevel = 3;	// 총기 레벨의 최대값

	UPROPERTY()
	AGunBaseActor* Gun = nullptr;		// 총기 클래스의 인스턴스
};
