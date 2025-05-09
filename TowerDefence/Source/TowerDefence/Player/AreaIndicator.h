// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaIndicator.generated.h"

UCLASS()
class TOWERDEFENCE_API AAreaIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Area")
	void SetAreaRadius(float NewRadius);

	void Activate();
	void Deactivate();

private:
	UFUNCTION()
	void OnMouseClick();	// 마우스 클릭 이벤트 처리

	void UpdateLocation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* AreaMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float AreaRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Area")
	float HeightOffset = 300.0f;	// Z축 높이 조정

private:
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	const float DefaultRadius = 50.0f;				// 기본 반지름(가독성을 위해 유지)
	const float ScaleFactor = 1 / DefaultRadius;

};
