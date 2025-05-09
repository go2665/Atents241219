// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/OverlapResult.h" 
#include "TowerBaseActor.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Data/SkillData/DEPRECATED_SkillDataAsset.h"
#include "HeroTowerActor.generated.h"

class AEnemyBase;
class APlayerSpectatorPawn;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AHeroTowerActor : public ATowerBaseActor
{
	GENERATED_BODY()
	
public:
	AHeroTowerActor();

	UFUNCTION(BlueprintCallable, Category = "Tower|Hero")
	void SelectingSkillLocation();

	UFUNCTION(BlueprintCallable, Category = "Tower|Hero")
	void UseSkill(FVector InLocation);

	virtual void Tick(float DeltaTime) override;	

protected:	
	virtual void BeginPlay() override;
	virtual void OnCancelClicked() override;
	inline virtual UTowerBuffComponent* GetBuffComponent() const override { return HeroBuffComponent; }	// 버프 컴포넌트 반환

private:
	//void FindEnemiesInRadius(FVector Center, float Radius, TArray<AEnemyBase*>& OutActors);
	//void FindTowersInRadius(FVector Center, float Radius, TArray<ATowerBaseActor*>& OutActors);

	// T 타입에 해당하는 적이나 타워를 찾는 함수
	template<typename T>
	void FindActorsInRadius(FVector Center, float Radius, TArray<T*>& OutActors)
	{
		// 출력 파라메터 초기화
		OutActors.Empty();
		// 결과를 저장할 배열  
		TArray<FOverlapResult> OverlapResults;
		
		// T 타입에 따라 적이나 타워의 충돌 채널을 설정
		ECollisionChannel CollisionChannel;
		if (std::is_same<T, AEnemyBase>::value)
		{
			CollisionChannel = ECollisionChannel::ECC_GameTraceChannel1; // 적
		}
		else if (std::is_same<T, ATowerBaseActor>::value)
		{
			CollisionChannel = ECollisionChannel::ECC_GameTraceChannel2; // 타워
		}
		else
		{
			return;
		}
		
		// 월드에서 Overlap 검사  
		bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(
			OverlapResults,
			Center,
			FQuat::Identity,
			FCollisionObjectQueryParams(CollisionChannel),
			FCollisionShape::MakeCapsule(Radius, 200.0f) // 200.0f은 타워 높이가 150 정도이므로 여유분을 주기 위해 설정
		);
		if (bHasOverlaps)
		{
			for (const FOverlapResult& Result : OverlapResults)
			{
				if (AActor* Actor = Result.GetActor())
				{
					if (T* CastedActor = Cast<T>(Actor))
					{
						OutActors.Add(CastedActor);
					}
				}
			}
		}

		// 디버그용으로 구체를 그려줌 (선택 사항)  
		DrawDebugCapsule(GetWorld(), Center, 200.0f, Radius, FQuat::Identity, FColor::Green, false, 2.0f);
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTowerBuffComponent* HeroBuffComponent = nullptr;	// 버프 컴포넌트 저장할 변수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Hero")
	TArray<U_DEPRECATED_SkillDataAsset*> SkillDataAssets;

private:
	bool bIsSelectingSkillLocation = false;	// 스킬 사용 위치 선택 중인지를 표시하는 변수

	float SkillCoolTime = 0.0f;

	UPROPERTY()
	APlayerSpectatorPawn* Player = nullptr;	// 플레이어 스펙테이터 폰을 저장할 변수
	
};

