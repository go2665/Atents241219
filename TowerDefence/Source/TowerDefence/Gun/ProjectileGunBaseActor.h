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

};
