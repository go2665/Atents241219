#pragma once

#include "CoreMinimal.h"

// 공격 우선 순위 타입 enum	
UENUM(BlueprintType)
enum class EFireOrder : uint8
{
	Discovery = 0	UMETA(DisplayName = "Discovery"),	// 발견 순서
	Distance		UMETA(DisplayName = "Distance"),	// 거리 순서
};

// 대미지 속성 타입 enum	
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None		UMETA(DisplayName = "None"),
	Fire		UMETA(DisplayName = "Fire"),
	Ice			UMETA(DisplayName = "Ice"),
	Lightning	UMETA(DisplayName = "Lightning"),
	Poison		UMETA(DisplayName = "Poison"),
	// Add more attribute types as needed
};

// 이펙트 종류를 나타내는 타입 enum	
UENUM(BlueprintType)
enum class EEffectTarget : uint8
{
	Friendly	UMETA(DisplayName = "Friendly"),
	Hostile		UMETA(DisplayName = "Hostile"),
};

// 버프/디버프 종류를 나타내는 타입 enum
UENUM(BlueprintType)
enum class EEffectType : uint8
{
	None				UMETA(DisplayName = "None"),

	// 버프 타입
	BuffStart = 1		UMETA(Hidden),	
	Buff_RapidFire		UMETA(DisplayName = "Buff RapidFire"),	// 발사 속도 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	Buff_LongRange		UMETA(DisplayName = "Buff LongRange"),	// 사거리 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	Buff_PowerUp		UMETA(DisplayName = "Buff PowerUp"),	// 공격력 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	Buff_Hero			UMETA(DisplayName = "Buff Hero"),		// 영웅 버프. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	// Add more buff types as needed
	BuffEnd				UMETA(Hidden),

	// 디버프 타입
	DebuffStart = 100	UMETA(Hidden),
	Debuff_Slow			UMETA(DisplayName = "Debuff Slow"),		// 이동 속도 감소. 일정시간 동안 작동, 누적 안됨, 추가시 시간 연장
	Debuff_Stun			UMETA(DisplayName = "Debuff Stun"),		// 스턴. 일정시간 동안 작동, 누적 안됨, 추가시 시간 연장
	Debuff_DotDamage	UMETA(DisplayName = "Debuff DotDamage"),// 지속 피해. 일정시간 동안 작동, 누적 됨, 추가시 시간 연장
	// Add more debuff types as needed
	DebuffEnd			UMETA(Hidden),
};

// 이펙트가 주는 모디파이어 타입 enum
UENUM(BlueprintType)
enum class EEffectModifier : uint8
{
	FireRate	UMETA(DisplayName = "FireRate"),	// 공격 속도
	FireRange	UMETA(DisplayName = "FireRange"),	// 사정거리
	Attack		UMETA(DisplayName = "Attack"),		// 데미지(공격력)
	TakeDamage	UMETA(DisplayName = "TakeDamage"),	// 받는 데미지
	MoveSpeed	UMETA(DisplayName = "MoveSpeed"),	// 이동 속도
	Defence		UMETA(DisplayName = "Defence"),		// 방어력
	// Add more modifier types as needed
	COUNT		UMETA(Hidden),	// 모디파이어 타입 개수
};

// 풀링될 액터 타입 enum
UENUM(BlueprintType)
enum class EPooledActorType : uint8
{
	None					UMETA(Hidden),									// 코드 처리용
	ProjectileArrow			UMETA(DisplayName = "ProjectileArrow"),			// 화살 발사체
	ProjectileBoulder		UMETA(DisplayName = "ProjectileBoulder"),		// 바위 발사체
	ProjectileBullet		UMETA(DisplayName = "ProjectileBullet"),		// 총알 발사체
	ProjectileCannonBall	UMETA(DisplayName = "ProjectileCannonBall"),	// 대포알 발사체
	EnemySpeed				UMETA(DisplayName = "EnemySpeed"),				// 적 스피드
	EnemyTanker				UMETA(DisplayName = "EnemyTanker"),				// 적 탱커
};

UENUM(BlueprintType)
enum class EShootType : uint8
{
	HitScan		UMETA(DisplayName = "HitScan"),	// 히트스캔
	Projectile	UMETA(DisplayName = "Projectile"),// 발사체
};