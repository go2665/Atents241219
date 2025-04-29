// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerBuilder.generated.h"

UCLASS()
class TOWERDEFENCE_API ATowerBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBuilder();
	inline ATower* GetTower() const { return Tower; }

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "TowerBuilder")
	void Test_BuildTower(int32 TowerIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TowerBuilder")
	TArray<TSubclassOf<ATower>> TowerClasses;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TowerBuilder", meta = (AllowPrivateAccess = "true"))
	ATower* Tower = nullptr;

};
