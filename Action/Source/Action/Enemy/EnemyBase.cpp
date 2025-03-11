// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "../Framework/ActionGameMode.h"
#include "../Item/DropItemBase.h"
#include "../Item/FDropItemDataTableRow.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	bIsAlive = true;
}

float AEnemyBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth -= ActualDamage;
	if (bIsAlive && CurrentHealth <= 0)
	{
		Die();
	}

	return ActualDamage;
}

void AEnemyBase::Die()
{
	bIsAlive = false;	// 죽었음을 표시
	OnDie.Broadcast();	// 델리게이트로 죽음을 알림

	DropItems();		// 아이템 드랍
	
	Destroy();			// 액터 삭제
}

void AEnemyBase::DropItems()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DropItems"));
	if (DropItemDataTable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Table Check OK"));

		UWorld* World = GetWorld();
		AActionGameMode* GameMode = Cast<AActionGameMode>(World->GetAuthGameMode());

		FVector DropLocation = GetActorLocation() + FVector::UpVector * 100.0f;
		FVector LaunchVelocity = FVector::UpVector * ItemLaunchPower;	

		//  - 아이템을 랜덤한 방향으로 던지기(or 액터 위치 주변에 아이템 생성)
		//  - 동전 1~3개 랜덤으로 드랍, 힐 오브 10%확률로 드랍
		//int GoldCount = FMath::RandRange(1, 3);
		//for (int i = 0; i < GoldCount; ++i)
		//{
		//	// 동전 생성
		//	ADropItemBase* Item = GameMode->GetDropItem(EItemType::GoldCoin, DropLocation);
		//	if (Item)
		//	{
		//		LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(-15.0f, 15.0f), FVector::RightVector);
		//		LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(0.0f, 360.0f), FVector::UpVector);
		//		DrawDebugLine(World, DropLocation, DropLocation + LaunchVelocity, FColor::Green, false, 3.0f);
		//		Item->SetVelocity(LaunchVelocity);						
		//	}
		//}
		//
		//if (FMath::FRand() < 0.1f)
		//{
		//	// 힐 오브 생성
		//	ADropItemBase* Item = GameMode->GetDropItem(EItemType::HealOrb, DropLocation);
		//	if (Item)
		//	{
		//		LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(-15.0f, 15.0f), FVector::RightVector);
		//		LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(0.0f, 360.0f), FVector::UpVector);
		//		DrawDebugLine(World, DropLocation, DropLocation + LaunchVelocity, FColor::Green, false, 3.0f);
		//		Item->SetVelocity(LaunchVelocity);
		//	}
		//}	
	
		TMap<FName, uint8*> RowMap = DropItemDataTable->GetRowMap();

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
			FString::Printf(TEXT("Table map Size : %d"), RowMap.Num()));
		

		for (auto& Elem : RowMap)
		{
			FDropItemDataTableRow* Row = (FDropItemDataTableRow*)Elem.Value;
			if (FMath::FRand() < Row->Chance)
			{
				ADropItemBase* Item = GameMode->GetDropItem(Row->ItemType, DropLocation);
				if (Item)
				{
					LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(-15.0f, 15.0f), FVector::RightVector);
					LaunchVelocity = LaunchVelocity.RotateAngleAxis(FMath::RandRange(0.0f, 360.0f), FVector::UpVector);
					DrawDebugLine(World, DropLocation, DropLocation + LaunchVelocity, FColor::Green, false, 3.0f);
					Item->SetVelocity(LaunchVelocity);
				}
			}
		}
		//  - 데이터 테이블을 이용해서 아이템 드랍 확률 기록하고 드랍할 때 사용하기
		//		- 데이터 테이블을 만드려면 FTableRowBase를 상속받은 구조체 필요
		//  - 아이템은 5초 후 사라짐(DropItemBase 클래스 수정)

	}
}
