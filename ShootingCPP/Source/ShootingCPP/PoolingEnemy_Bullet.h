// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingObjectBase.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "PoolingEnemy_Bullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Bullet : public APoolingObjectBase
{
	GENERATED_BODY()
	
public:
	APoolingEnemy_Bullet();

protected:
	virtual void BeginPlay() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

private:
	// 총알이 무언가와 겹쳤을 때 호출되는 함수
	UFUNCTION()
	void OnBulletOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// 총알 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UStaticMeshComponent* BulletMesh = nullptr;

	// 총알 포워드 표시용	화살표
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UArrowComponent* Arrow = nullptr;

	// 총알 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UProjectileMovementComponent* ProjectileMovement = nullptr;

protected:
	// 총알 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float MoveSpeed = 200.0f;

	// 총알의 수명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float LifeTime = 20.0f;

	// 총알이 플레이어에게 주는 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Damage = 3.0f;

	// 총알이 터지는 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	UNiagaraSystem* SelfExplosionEffect = nullptr;

private:
	// 초기화 여부(첫번째 비활성화 때 터지는 이팩트 나오는 것 방지용)
	bool bIsInitialized = false;

	// 수명 체크용 타이머
	UPROPERTY()
	FTimerHandle LifeTimeTimer;
};
