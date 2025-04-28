// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
class TOWERDEFENCE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// 발사체 데이터 초기화(스폰 직후에 반드시 호출 되어야 함)
	UFUNCTION(BlueprintCallable, Category = "Shot")
	void OnInitialize(const AActor* InTarget, 
		const UShotDataAsset* InShotData, int32 InLevel,
		float InDamage,
		bool InbShowDebugInfo = false,	// 나중에 파라메터 마지막으로 보낼것
		float InEffectModifier = 1.0f );

	// 이 발사체에 맞은 적 기록
	void AddHitEnemy(AEnemy* InEnemy) { HitEnemies.Add(InEnemy); }	

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
	const AActor* TargetActor = nullptr;

	// 발사체 레벨
	int32 ShotLevel = 0;	

	// 발사체가 날아갈 목표 위치
	FVector TargetLocation = FVector::ZeroVector;	

	// 발사체가 적에게 줄 데미지
	float Damage = 1.0f;

	// 발사체가 적에게 줄 효과에 대한 배율(디버프 배율)
	float EffectModifier = 1.0f;					

	// 디버그 정보 표시 여부
	bool bShowDebugInfo = false;	

	// 이 발사체에 맞은 적들(범위 공격일 때만 사용)
	TArray<AEnemy*> HitEnemies;
};
