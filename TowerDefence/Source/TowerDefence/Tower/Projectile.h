// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDefence/Framework/ObjectPool/PoolableActor.h"
#include "Projectile.generated.h"

class AEnemy;
class UShotDataAsset;
class UProjectileMovementComponent;
struct FShotLevelData;

//#define GET_SHOT_LEVEL_DATA ShotData->LevelData[ShotLevel]	// 메크로 예시

/**
 * 발사체 클래스. 블루프린트는 외형과 이동속도만 설정. 그 외 데이터는 OnInitialize에서 설정한 UShotDataAsset로 설정됨.
 */ 

UCLASS()
class TOWERDEFENCE_API AProjectile : public AActor, public IPoolableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Tick(float DeltaTime) override;

protected:

public:	

	// 발사체가 풀에서 꺼내질 때 마다 해야할일(초기 트랜스폼, 샷데이터, 대상, 레벨, 데미지 설정)
	UFUNCTION(BlueprintCallable, Category = "Shot")
	void OnSpawn(
		const FTransform& InTransform, 
		const UShotDataAsset* InShotData,
		const AEnemy* InTarget, 
		int32 InLevel, 
		float InDamage, 
		bool InbShowDebugInfo = false);

	// 이 발사체에 맞은 적 기록
	void AddHitEnemy(AEnemy* InEnemy) { HitEnemies.Add(InEnemy); }	

	// IPoolableActor을(를) 통해 상속됨 ---------------------------------

	// 오브젝트가 풀에서 생성될 때 호출(초기화)
	void OnInitialize() override;

	// 오브젝트가 풀에서 꺼내질 때 호출(활성화 때)
	void OnActivate() override;

	// 오브젝트가 풀로 돌아갈 때 호출(비활성화 때)
	void OnDeactivate() override;

	inline EPooledActorType GetPoolType() const override { return PoolType; };
	inline void SetPoolType(EPooledActorType InType) override { PoolType = InType; };
	// -----------------------------------------------------------------

protected:
	// 적과의 겹침을 판단하기 위한 함수
	UFUNCTION()
	void OnOverlapEnemy(AActor* OverlappedActor, AActor* OtherActor);	

	// 실제 겹친 적에 대한 처리를 하는 함수
	void DamageToEnemy(AEnemy* HitEnemy);	

	// 현재 범위 안에 있는 모든 적에게 데미지 적용
	void DamageToArea(AActor* InIgnore);

private:
	const FShotLevelData& GetShotLevelData() const;

	// 적에게 맞았을 때의 처리를 수행하는 함수
	void OnHitEnemy(AEnemy* InHitEnemy);	

protected:
	// 발사체 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;	

	// 발사체 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh = nullptr;	

	// 발사체 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	const UShotDataAsset* ShotData = nullptr;	

	// 발사체 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Shot")
	float MoveSpeed = 300.0f;

private:
	// 발사체가 날아갈 타겟 액터(적)
	UPROPERTY()
	const AEnemy* TargetEnemy = nullptr;

	// 발사체 레벨
	int32 ShotLevel = 0;	

	// 발사체가 날아갈 목표 위치
	FVector TargetLocation = FVector::ZeroVector;	

	// 발사체가 적에게 줄 데미지
	float Damage = 1.0f;				

	// 디버그 정보 표시 여부
	bool bShowDebugInfo = false;	

	// 이 발사체에 맞은 적들(범위 공격일 때만 사용)
	TArray<AEnemy*> HitEnemies;

	// 타겟이 죽었는지 여부(발사체가 날아가는 도중에 적이 죽으면 직선으로 날아감)
	bool bTargetDie = false;	

	// 활성화 여부
	bool bActivate = true;

	// 풀액터 타입(발사체 타입중 하나 선택)
	EPooledActorType PoolType = EPooledActorType::None;	

	// 수명 타이머 핸들
	FTimerHandle LifeTimerHandle;	

};
