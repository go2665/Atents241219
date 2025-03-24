// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWeaponActor.generated.h"

UCLASS()
class ACTION_API AEnemyWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyWeaponActor();

	// 무기의 활성화 여부를 설정한다.
	void SetActivate(bool bActivate);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 10.0f;

private:
	UStaticMeshComponent* WeaponMesh = nullptr;

};
