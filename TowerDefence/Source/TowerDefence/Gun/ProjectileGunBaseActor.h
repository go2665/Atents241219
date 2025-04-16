// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBaseActor.h"
#include "ProjectileGunBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AProjectileGunBaseActor : public AGunBaseActor
{
	GENERATED_BODY()
	
protected:
	virtual void Shoot() override;
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<class AShotProjectileBase> ProjectileClass;	// 발사체 클래스
};
