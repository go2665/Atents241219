// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDamageTypeBase.h"
#include "DamageRed.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UDamageRed : public UProjectileDamageTypeBase
{
	GENERATED_BODY()

public:
	UDamageRed()
	{
		Type = EDamageType::DT_Red;
	}	
};
