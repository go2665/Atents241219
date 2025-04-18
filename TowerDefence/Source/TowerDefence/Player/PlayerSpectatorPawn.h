// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "AreaIndicator.h"
#include "PlayerSpectatorPawn.generated.h"

class AAreaIndicator;
/**
 * 
 */
UCLASS()
class TOWERDEFENCE_API APlayerSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	// AreaIndicator 활성화
	inline void AreaIndicatorActivate() { AreaIndicator->Activate(); }	

	// AreaIndicator 위치 가져오기
	inline FVector GetAreaIndicatorLocation() const
	{
		return AreaIndicator ? AreaIndicator->GetActorLocation() : FVector::ZeroVector;
	}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaIndicator")
	TSubclassOf<AAreaIndicator> AreaIndicatorClass;	// AreaIndicator 클래스

private:
	UPROPERTY()
	AAreaIndicator* AreaIndicator = nullptr;	// AreaIndicator를 저장할 변수
};
