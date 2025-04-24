// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectComponent.h"
#include "TowerDefence/Tower/Effect/EffectBase.h"

// Sets default values for this component's properties
UEffectComponent::UEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEffectComponent::AddEffect(EEffectType InType)
{
	// 이미 존재하는 버프는 중첩시킨다.
	for (auto& Effect : EffectList)
	{
		if (Effect->GetEffectType() == InType)
		{
			Effect->OnStack();
			OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Already Exist Effect => [%s]"), 
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Effect->GetBuffType()));
			return;
		}
	}

	// 이전에 없던 버프는 새롭게 생성하고 리스트에 추가한다.
	UEffectBase* NewEffect = CreateEffect(InType);
	if (NewEffect)
	{
		const UEffectDataAsset* EffectData = nullptr;
		if (EffectDataMap.Contains(InType))		// InType이 Map에 설정되어 있어야 동작
		{
			EffectData = EffectDataMap[InType];	// 이팩트 데이터 에셋을 찾는다.

			NewEffect->OnInitialize(InType, EffectData, GetOwner());	// 이팩트 데이터 에셋 설정하면서 초기화
			EffectList.AddUnique(NewEffect);	// 이팩트 리스트에 추가
			NewEffect->OnEffectExpire.BindLambda([this](UEffectBase* ExpiredEffect)
				{
					RemoveEffect(ExpiredEffect->GetEffectType());
				}
			);	// 이팩트 만료 델리게이트 바인딩
			NewEffect->OnBegin();				// 버프 시작
			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Create New Effect => [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(NewEffect->GetBuffType()));

			OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
		}
	}
}

void UEffectComponent::RemoveEffect(EEffectType InType)
{
	// 특정 버프 하나를 제거한다.
	for (int32 i = EffectList.Num() - 1; i > -1; i--)	// TArray에서 제거 작업은 뒤에서부터 진행하는 것이 유리하다.
	{
		if (EffectList[i]->GetEffectType() == InType)
		{
			EffectList.RemoveAt(i);
			EffectList[i]->OnEnd();

			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Remove Buff => [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Type));

			OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
			break;
		}
	}
}

UEffectBase* UEffectComponent::CreateEffect(EEffectType InType)
{
	UEffectBase* NewEffect = nullptr;	
	if (EffectDataMap.Contains(InType))		// InType이 Map에 설정되어 있어야 동작
	{
		const UEffectDataAsset* EffectData = EffectDataMap[InType];	// 이팩트 데이터 에셋을 찾는다.

		// 버프 타입에 따라 적절한 버프 클래스를 생성한다.(버프가 걸린쪽에서 생성)
		FName EnumFName = FName(*UEnum::GetValueAsString(InType));
		NewEffect = NewObject<UEffectBase>(this, EffectData->EffectClass, EnumFName);
	}

	return NewEffect;
}

void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i < EffectList.Num(); i++)
	{
		EffectList[i]->Tick(DeltaTime);	// 이팩트 틱 진행
	}
}

