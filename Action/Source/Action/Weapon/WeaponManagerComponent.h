// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponType.h"
#include "WeaponManagerComponent.generated.h"


//UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS()
class ACTION_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	class AWeaponActor* GetWeapon(EWeaponType WeaponType) const {
		return WeaponInstances[static_cast<uint8>(WeaponType)];
	};
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AActionPlayerCharacter* OwnerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<TSubclassOf<class AWeaponActor>> WeaponClasses;

	UPROPERTY()
	TArray<class AWeaponActor*> WeaponInstances;
			
	
	// 무기를 선택해서 플레이어에게 장착시킨다.
};
