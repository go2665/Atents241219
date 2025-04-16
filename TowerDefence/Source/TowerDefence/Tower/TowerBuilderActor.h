// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerBuilderActor.generated.h"

class ATowerBaseActor;

UCLASS()
class TOWERDEFENCE_API ATowerBuilderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATowerBuilderActor();

	inline ATowerBaseActor* GetTower() const { return Tower; }

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "TowerBuilder")
	void Test_BuildTower(int32 TowerIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower")
	TArray<TSubclassOf<ATowerBaseActor>> TowerClasses;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tower", meta = (AllowPrivateAccess = "true"))
	ATowerBaseActor* Tower = nullptr;
};
