// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferComponent.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Tower/TowerBuilder.h"
#include "Kismet/KismetSystemLibrary.h"

UBufferComponent::UBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	ensure(BufferDataAsset != nullptr);
	if (BufferDataAsset)
	{
		// 타워 레벨업 델리게이트에 람다함수 추가
		Tower = Cast<ATower>(GetOwner()); // 소유자(타워) 가져오기
		ensure(Tower != nullptr);
		bShowDebugInfo = Tower->GetShowDebugInfo();
		if (Tower)
		{
			Tower->OnTowerLevelUp.AddLambda(
				[this](int32 InLevel)
				{
					int32 NewLevel = FMath::Min(BufferDataAsset->LevelData.Num(), InLevel); //최대 LevelData 개수만큼
					if (Level != NewLevel)	// 레벨이 변경되었을 때만 처리
					{
						Level = NewLevel;
						ResetBuffTimer();	// 레벨에 맞춰 타이머 재 설정
					}
				}
			); // 레벨업 델리게이트에 추가
		}

		// 주변에 있는 타워 배치 가능한 액터(ATowerBuilder)를 찾아서 TowerBuilderList에 저장해 놓기
		TowerBuilderList.Empty(); // 초기화
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<AActor*> OverlappingActors;
		bool IsOverlapped = UKismetSystemLibrary::SphereOverlapActors( // 구 안에 있는 모든 ATowerBuilder를 찾는다.
			GetWorld(),
			GetOwner()->GetActorLocation(),
			GetRadius(),
			ObjectTypes,
			ATowerBuilder::StaticClass(),
			TArray<AActor*>(),
			OverlappingActors
		);

		TowerBuilderList.Reserve(OverlappingActors.Num());	// 미리 메모리 할당
		if (IsOverlapped)
		{
			for (AActor* Actor : OverlappingActors)
			{
				ATowerBuilder* TowerBuilder = Cast<ATowerBuilder>(Actor);
				if (TowerBuilder)
				{
					TowerBuilderList.Add(TowerBuilder);		// 찾은 TowerBuilder들을 캐스팅해서 저장
				}
			}
		}

		// 타이머를 이용해 주기적으로 버프 걸기
		ResetBuffTimer();
	}
	
}

void UBufferComponent::AddBuffToAround()
{
	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 순회하면서 버프를 건다.
	for (ATowerBuilder* TowerBuilder : TowerBuilderList)
	{
		if (TowerBuilder 
			&& TowerBuilder->GetTower()				// 주변에 있는 TowerBuilder에 타워가 있고
			&& TowerBuilder->GetTower() != Tower)	// 내 타워가 아닌 경우에만 버프를 건다.
		{			
			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] is add buff to [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *TowerBuilder->GetTower()->GetActorNameOrLabel());

			TowerBuilder->GetTower()->AddEffect(GetBuffType(), Level);
		}
	}

	if (bShowDebugInfo)
	{
		DrawDebugSphere(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			GetRadius(), 12, FColor::Red, false, GetInterval() * 0.9f); // 디버그 구 그리기
	}
}

void UBufferComponent::ResetBuffTimer()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(BufferTimerHandle); // 기존 타이머 정리
	TimerManager.SetTimer( // 새롭게 시작
		BufferTimerHandle,
		this,
		&UBufferComponent::AddBuffToAround,
		GetInterval(),
		true,
		0.5f
	);	
}


