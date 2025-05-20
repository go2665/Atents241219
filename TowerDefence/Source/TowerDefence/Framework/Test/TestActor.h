// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class TOWERDEFENCE_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable, Category = "Test")
	void TestFunction();

};
