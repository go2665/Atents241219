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


// 적이 맞았을 때 애니메이션을 재생한다.
//  1. 맞았을 때 애니메이션을 재생(Enemy_Hit)
//  2. 데미지 팝업
// 플레이어에가 맞아 죽을 수 있다.
//	1. 죽었을 때 애니메이션을 재생(Death, Death_Small_03, Death_Small_05 중 랜덤 재생)
//  2. 죽는 애니메이션이 끝났을 때 아이템 드랍
//	3. 죽는 애니메이션이 끝났을 때 바닥으로 가라 앉다가 사라지기