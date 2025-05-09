// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "AreaIndicator.h"
#include "PlayerSpectatorPawn.generated.h"

class AAreaIndicator;
class ATower;
class ATowerDefencePlayerController;

/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API APlayerSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	APlayerSpectatorPawn();

	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

	// AreaIndicator 활성화
	inline void AreaIndicatorActivate() { AreaIndicator->Activate(); }	

	// AreaIndicator 위치 가져오기
	inline FVector GetAreaIndicatorLocation() const
	{
		return AreaIndicator ? AreaIndicator->GetActorLocation() : FVector::ZeroVector;
	}

	inline const ATower* GetTemporaryHero() const
	{
		return TemporaryHero;
	}

	inline void SetTemporaryHero(ATower* InTemporaryHero)
	{
		TemporaryHero = InTemporaryHero;
	}

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnMouseClick();	// 마우스 클릭 이벤트 핸들러

	// 임시 영웅타워의 위치를 업데이트
	void OnTemporaryHeroUpdata();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaIndicator")
	TSubclassOf<AAreaIndicator> AreaIndicatorClass;	// AreaIndicator 클래스

private:
	UPROPERTY()
	AAreaIndicator* AreaIndicator = nullptr;	// AreaIndicator를 저장할 변수

	UPROPERTY()
	ATower* TemporaryHero = nullptr;

	UPROPERTY()
	ATowerDefencePlayerController* PlayerController = nullptr;
};
