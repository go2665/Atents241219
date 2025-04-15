// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuffComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TowerDefence/Tower/TowerBuilderActor.h"
#include "TowerDefence/Tower/TowerBaseActor.h"
#include "TowerDefence/Tower/Buff/TowerBuff_RapidFire.h"
#include "TowerDefence/Tower/Buff/TowerBuff_LongRange.h"
#include "TowerDefence/Tower/Buff/TowerBuff_PowerUp.h"

// Sets default values for this component's properties
UTowerBuffComponent::UTowerBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTowerBuffComponent::OnAddedBuff(ETowerBuffType Type)
{
	// 이미 존재하는 버프는 기간만 리셋한다.
	for (auto& Buff : BuffList)
	{
		if (Buff->GetBuffType() == Type)
		{
			Buff->ResetDuration();
			return;
		}
	}

	// 이전에 없던 버프는 새롭게 생성하고 리스트에 추가한다.
	UTowerBuffBase* NewBuff = CreateBuff(Type);
	if (NewBuff)
	{
		NewBuff->OnInitialize(BuffDataAsset);
		NewBuff->OnBuffBegin();
		BuffList.AddUnique(NewBuff);
	}
}

void UTowerBuffComponent::OnRemoveBuff(ETowerBuffType Type)
{
	// 버프 하나를 제거한다.
	for (int32 i = 0; i < BuffList.Num(); i++)
	{
		if (BuffList[i]->GetBuffType() == Type)
		{
			BuffList[i]->OnBuffEnd();
			BuffList.RemoveAt(i);
			break;
		}
	}
}

// Called when the game starts
void UTowerBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 찾아서 TowerBuildActorList에 저장해 놓기
	TowerBuilderList.Empty(); // 초기화
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> OverlappingActors;
	bool IsOverlapped = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetOwner()->GetActorLocation(),
		Radius,
		ObjectTypes,
		ATowerBuilderActor::StaticClass(),
		TArray<AActor*>(),
		OverlappingActors
	);
	TowerBuilderList.Reserve(OverlappingActors.Num()); // 미리 메모리 할당
	if (IsOverlapped)
	{
		for (AActor* Actor : OverlappingActors)
		{
			ATowerBuilderActor* TowerBuilderActor = Cast<ATowerBuilderActor>(Actor);
			if (TowerBuilderActor)
			{
				TowerBuilderList.Add(TowerBuilderActor);
			}
		}
	}	

	// 주기적으로 버프 걸기
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (BuffDataAsset)
	{
		// 주기적으로 버프를 걸기 위해 타이머를 설정한다.
		TimerManager.SetTimer(
			BuffTimerHandle,
			this,
			&UTowerBuffComponent::AddBuffToAround,
			Interval,
			true,
			0.5f
		);
	}
	else
	{
		TimerManager.ClearTimer(BuffTimerHandle);
	}
}

// Called every frame
void UTowerBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 시간이 다된 버프를 제거한다.
	RemoveIndices.Empty();
	for (int32 i = 0; i < BuffList.Num(); i++)
	{
		BuffList[i]->OnBuffTick(DeltaTime);
		if (BuffList[i]->GetCurrentDuration() <= 0.0f)
		{
			BuffList[i]->OnBuffEnd();
			RemoveIndices.Add(i);
		}
	}
	for (int32 i = RemoveIndices.Num() - 1; i >= 0; i--)
	{
		//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
		//UE_LOG(LogTemp, Warning, TEXT("[%s] Success RemoveBuff : [%s]"),
		//	*TimeString, *UEnum::GetValueAsString(BuffList[RemoveIndices[i]]->GetBuffType()))
		BuffList.RemoveAt(RemoveIndices[i]);
	}
}

void UTowerBuffComponent::AddBuffToAround()
{
	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 순회하면서 버프를 건다.
	for (ATowerBuilderActor* TowerBuilder : TowerBuilderList)
	{
		if (TowerBuilder && TowerBuilder->GetTower())
		{
			TowerBuilder->GetTower()->AddBuff(BuffDataAsset->BuffType);
		}
	}
}

UTowerBuffBase* UTowerBuffComponent::CreateBuff(ETowerBuffType Type)
{
	UTowerBuffBase* NewBuff = nullptr;
	switch (Type)
	{
	case ETowerBuffType::RapidFire:
		NewBuff = NewObject<UTowerBuff_RapidFire>(this, UTowerBuff_RapidFire::StaticClass(), TEXT("RapidFire"));
		break;
	case ETowerBuffType::LongRange:
		NewBuff = NewObject<UTowerBuff_LongRange>(this, UTowerBuff_LongRange::StaticClass(), TEXT("LongRange"));
		break;
	case ETowerBuffType::PowerUp:
		NewBuff = NewObject<UTowerBuff_PowerUp>(this, UTowerBuff_PowerUp::StaticClass(), TEXT("PowerUp"));
		break;
		// Add more cases for other buff types as needed
	default:
		break;
	}

	return NewBuff;
}

