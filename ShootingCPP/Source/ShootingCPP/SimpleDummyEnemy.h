// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDamageType.h"
#include "SimpleDummyEnemy.generated.h"

UCLASS()
class SHOOTINGCPP_API ASimpleDummyEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimpleDummyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 이 적이 데미지를 받았을 때 호출되는 함수
	UFUNCTION()
	void OnAnyDamage( AActor* DamagedActor, float Damage, 
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

protected:
	// 스테틱 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Dummy Enemy")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dummy Enemy")
	EDamageType WeakType = EDamageType::DT_Red;

	UPROPERTY(EditAnywhere, Category = "Dummy Enemy")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Dummy Enemy")
	float MaxHealth = 100.0f;
};
