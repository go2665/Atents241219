// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "TowerDefence/Tower/Projectile.h"
#include "TowerDefence/Framework/ObjectPool/ObjectPoolSubsystem.h"

// Sets default values
AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	this->OnActorBeginOverlap.AddDynamic(this, &AKillZone::OnProjectileOverlapBegin);
}

void AKillZone::OnProjectileOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("KillZone: Overlapped with %s"), *OtherActor->GetName());
	if (OtherActor->IsA<AProjectile>())
	{
		//UE_LOG(LogTemp, Warning, TEXT("[%s] Destroy"), *OtherActor->GetName());
		
		GetWorld()->GetSubsystem<UObjectPoolSubsystem>()->ReleaseObject(OtherActor);
	}
}
