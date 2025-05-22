// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"

void ATestActor::TestFunction()
{
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (ObjectPoolSubsystem)
	{
		ObjectPoolSubsystem->Test();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectPoolSubsystem not found"));
	}
}

	{