// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Action/Weapon/EnemyWeaponActor.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ANormalEnemy : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	ANormalEnemy();

	UFUNCTION(BlueprintCallable)
	void Attack();

	inline void SetWeaponCollisionActivate(bool bActivate) { if (Weapon) Weapon->SetActivate(bActivate); };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxWalkSpeed = 400.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UChildActorComponent* ChildActorComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AEnemyWeaponActor> WeaponClass = nullptr;

	// 공격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* AttackMontage = nullptr;

private:
	UPROPERTY()
	AEnemyWeaponActor* Weapon = nullptr;

};