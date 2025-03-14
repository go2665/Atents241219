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
	
	// UI에서 우클릭하면 호출됨
	// 1. 인벤토리 장비 슬롯에 이미 아이템이 장비되어 있는 경우
	//	1.1. 장비된 아이템을 해제한다.
	//	1.2. 이 아이템을 장비한다.
	// 2. 인벤토리 장비 슬롯에 아이템이 장비되어 있지 않은 경우
	//	2.1. 이 아이템을 장비한다.
}
