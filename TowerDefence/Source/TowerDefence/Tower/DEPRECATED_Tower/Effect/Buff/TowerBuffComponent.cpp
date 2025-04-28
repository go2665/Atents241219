// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBuffComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/TowerBuilderActor.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/TowerBaseActor.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Buff/TowerBuffBase.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Buff/TowerBuff_RapidFire.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Buff/TowerBuff_LongRange.h"
#include "TowerDefence/Tower/DEPRECATED_Tower/Effect/Buff/TowerBuff_PowerUp.h"

// Sets default values for this component's properties
UTowerBuffComponent::UTowerBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// UTowerBuffDataAsset* const Data : Data가 저장하고 있는 주소를 수정하는 것이 불가능하다.
// const UTowerBuffDataAsset* Data : Data가 저장하고 있는 주소에 있는 값을 수정하는 것이 불가능하다.

void UTowerBuffComponent::OnAddedBuff(const UTowerBuffDataAsset* InData)
{
	// 이미 존재하는 버프는 기간만 리셋한다.
	for (auto& Buff : BuffList)
	{
		if (Buff->GetBuffType() == InData->BuffType)
		{
			Buff->ResetDuration();

			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Already Exist Buff => [%s]"), 
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Buff->GetBuffType()));
			return;
		}
	}

	// 이전에 없던 버프는 새롭게 생성하고 리스트에 추가한다.
	UTowerBuffBase* NewBuff = CreateBuff(InData->BuffType);
	if (NewBuff)
	{
		NewBuff->OnInitialize(OwnerTower, InData);	// 버프 데이터 에셋 설정하면서 초기화
		BuffList.AddUnique(NewBuff);			// 버프 리스트에 추가
		CalculateTotalBuffModifiers();			// 버프 모디파이어 재계산

		NewBuff->OnBuffBegin();			// 버프 시작(재계산 된 결과를 적용)

		//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Create New Buff => [%s]"),
		//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(NewBuff->GetBuffType()));
	}
}

void UTowerBuffComponent::OnRemoveBuff(ETowerBuffType Type)
{
	// 특정 버프 하나를 제거한다.
	for (int32 i = 0; i < BuffList.Num(); i++)
	{
		if (BuffList[i]->GetBuffType() == Type)
		{
			BuffList.RemoveAt(i);
			CalculateTotalBuffModifiers();	// 버프 모디파이어 재계산
			BuffList[i]->OnBuffEnd();

			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Remove Buff => [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Type));
			break;
		}
	}
}

// Called when the game starts
void UTowerBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerTower = Cast<ATowerBaseActor>(GetOwner());

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
		// BuffDataAsset가 있으면 주기적으로 버프를 걸기 위해 타이머를 설정한다.
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
		// BuffDataAsset가 없으면 타이머를 제거한다.
		TimerManager.ClearTimer(BuffTimerHandle);
	}
}

// Called every frame
void UTowerBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
	RemoveIndices.Empty();
	for (int32 i = 0; i < BuffList.Num(); i++)
	{
		BuffList[i]->OnBuffTick(DeltaTime);	// 버프 틱 진행
		if (BuffList[i]->GetCurrentDuration() <= 0.0f)
		{
			// 시간이 다 된 버프를 종료하고 기록한다.
			BuffList[i]->OnBuffEnd();
			RemoveIndices.Add(i);
		}
	}

	// 버프 리스트에서 제거할 버프를 제거한다.
	for (int32 i = RemoveIndices.Num() - 1; i >= 0; i--)
	{
		//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
		//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Buff Timeout => [%s]"),
		//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(BuffList[RemoveIndices[i]]->GetBuffType()));

		BuffList.RemoveAt(RemoveIndices[i]);
	}

	if (RemoveIndices.Num() > 0)
	{
		CalculateTotalBuffModifiers();		// 버프 모디파이어 재계산	
	}
}

void UTowerBuffComponent::AddBuffToAround()
{
	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 순회하면서 버프를 건다.
	for (ATowerBuilderActor* TowerBuilder : TowerBuilderList)
	{
		if (TowerBuilder && TowerBuilder->GetTower()) 
		{
			// 주변에 있는 TowerBuilder에 타워가 있을 때
			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] is add buff to [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *TowerBuilder->GetTower()->GetActorNameOrLabel());

			TowerBuilder->GetTower()->AddBuff(BuffDataAsset);
		}
	}
}

UTowerBuffBase* UTowerBuffComponent::CreateBuff(ETowerBuffType Type)
{
	// 버프 타입에 따라 적절한 버프 클래스를 생성한다.(버프가 걸린쪽에서 생성)
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

void UTowerBuffComponent::CalculateTotalBuffModifiers()
{
	// 현재 적용된 버프의 모디파이어를 합산하는 함수
	TotalBuffModifiers.Empty();
	int32 BuffCount = static_cast<int32>(ETowerBuffModifier::COUNT);	// 모든 버프 모디파이어의 개수
	for (auto& Buff : BuffList)
	{
		// BuffList에 있는 모든 버프를 순회하면서 모디파이어를 곱연산으로 합산한다.
		for (int32 i = 0; i < BuffCount; i++)
		{
			ETowerBuffModifier ModifierType = static_cast<ETowerBuffModifier>(i);
			float ModifierValue = Buff->GetModifierValue(ModifierType);
			if (TotalBuffModifiers.Contains(ModifierType))
			{
				TotalBuffModifiers[ModifierType] *= ModifierValue;	// 모든 모디파이어는 곱연산으로 적용.
			}
			else
			{
				TotalBuffModifiers.Add(ModifierType, ModifierValue);
			}
		}
	}

	OwnerTower->RefreshBuffModifiers();	// 타워에 적용된 버프 모디파이어 재적용
}

