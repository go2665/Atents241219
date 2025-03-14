// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "../../Weapon/WeaponType.h"
#include "Action/Item/Interface/EquipableItem.h"
#include "ItemDataAsset_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UItemDataAsset_Weapon : public UItemDataAsset, public IEquipableItem
{
	GENERATED_BODY()
	
public:
	virtual void EquipItem(class IEquipTarget* Target) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon")
	EWeaponType WeaponType = EWeaponType::Axe;
};
