// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/OverlapResult.h" 
#include "TowerBaseActor.h"
#include "TowerDefence/Skill/SkillDataAsset.h"
#include "HeroTowerActor.generated.h"

class AEnemyBase;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API AHeroTowerActor : public ATowerBaseActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Tower|Hero")
	void UseSkill(FVector InLocation);

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
			FCollisionShape::MakeSphere(Radius)
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
	    DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Green, false, 2.0f);
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Hero")
	TArray<USkillDataAsset*> SkillDataAssets;

	
};

