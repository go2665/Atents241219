// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset_Weapon.h"
#include "Action/Player/EquipTarget.h"

void UItemDataAsset_Weapon::EquipItem(IEquipTarget* Target)
{
	// IEquipTarget 인터페이스를 구현한 대상(=Player)에게 아이템을 장비한다.
	if (Target)
	{
		Target->Equip(WeaponType);
	}
}
