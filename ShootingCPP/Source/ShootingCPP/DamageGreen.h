// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDamageTypeBase.h"
#include "DamageGreen.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UDamageGreen : public UProjectileDamageTypeBase
{
	GENERATED_BODY()

public:
	UDamageGreen()
	{
		Type = EDamageType::DT_Green;
	}
};
