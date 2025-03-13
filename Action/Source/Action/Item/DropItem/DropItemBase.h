// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropItemBase.generated.h"

UCLASS()
class ACTION_API ADropItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItemBase();

	void InitializeItemDataAsset(class UItemDataAsset* InItemDataAsset);
	void SetVelocity(const FVector& InVelocity);

protected:
	virtual void OnGetItem(AActor* InTarget) {};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnDropItemHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnDropItemOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ItemMesh = nullptr;

	// 물리 처리 콜리젼(떨어지는 동작)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* CollisionSphere = nullptr;

	// 강조 이펙트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* HighlightEffect = nullptr;
	
	// 아이템 데이터 에셋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class UItemDataAsset* ItemDataAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	float LifeSpan = 10.0f;

private:
	// 바닥에 떨어졌을 때 움직임 처리용 변수
	float ElapsedTime = 0.0f;

	// 활성화 여부(바닥에 떨어지면 활성화하고 습득 및 회전 시작)
	bool bIsActivated = false;

	// 초기화 여부
	bool bInitialized = false;
};
