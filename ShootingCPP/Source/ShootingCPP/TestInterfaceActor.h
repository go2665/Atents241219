// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestInterface.h"
#include "TestInterfaceActor.generated.h"

UCLASS()
class SHOOTINGCPP_API ATestInterfaceActor : public AActor, public ITestInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestInterfaceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TestNormalFunction() override;

	UFUNCTION(BlueprintNativeEvent)
	void TestNative();
	virtual void TestNative_Implementation() override;
};
