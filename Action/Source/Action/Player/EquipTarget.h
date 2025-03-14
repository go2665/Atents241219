// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Weapon/WeaponType.h"
#include "EquipTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 무기를 장비할 수 있는 폰에게 추가할 인터페이스
 */
class ACTION_API IEquipTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Equip(EWeaponType InWeaponType) = 0;
	virtual void UnEquip() = 0;
};
