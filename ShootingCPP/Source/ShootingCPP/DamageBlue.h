// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDamageTypeBase.h"
#include "DamageBlue.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API UDamageBlue : public UProjectileDamageTypeBase
{
	GENERATED_BODY()
	
public:
	UDamageBlue()
	{
		Type = EDamageType::DT_Blue;
	}
};
