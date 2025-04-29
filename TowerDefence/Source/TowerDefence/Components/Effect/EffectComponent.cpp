// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectComponent.h"
#include "TowerDefence/Tower/Effect/EffectBase.h"
#include "TowerDefence/Components/Effect/EffectTargetable.h"

// Sets default values for this component's properties
UEffectComponent::UEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();	// 이팩트 컴포넌트 시작
	
	// 이팩트가 적용될 타겟을 인터페이스로 저장한다.
	EffectTarget = TScriptInterface<IEffectTargetable>(GetOwner());	
}

bool UEffectComponent::AddEffect(EEffectType InType)
{
	// 이미 존재하는 이팩트는 중첩시킨다.
	for (auto& Effect : EffectList)
	{
		if (Effect->GetEffectType() == InType)
		{
			Effect->OnStack();
			//OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
			ApplyTotalModifiersToTarget(); // 모디파이어 다시 합산하고 대상에 적용
			
			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Already Exist Effect => [%s]"), 
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Effect->GetBuffType()));
			return true;
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

			//OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
			ApplyTotalModifiersToTarget(); // 모디파이어 다시 합산하고 대상에 적용
			return true;
		}
	}
	return false;
}

bool UEffectComponent::RemoveEffect(EEffectType InType)
{
	// 특정 버프 하나를 제거한다.
	for (int32 i = EffectList.Num() - 1; i > -1; i--)	// TArray에서 제거 작업은 뒤에서부터 진행하는 것이 유리하다.
	{
		if (EffectList[i]->GetEffectType() == InType)
		{
			EffectList[i]->OnEnd();
			EffectList.RemoveAt(i);

			//FString TimeString = FDateTime::FromUnixTimestamp(GetWorld()->TimeSeconds).ToString(TEXT("%H:%M:%S"));
			//UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Remove Buff => [%s]"),
			//	*TimeString, *GetOwner()->GetActorNameOrLabel(), *UEnum::GetValueAsString(Type));

			//OnEffectChanged.ExecuteIfBound(EffectList);	// 이팩트 변경 델리게이트 호출
			ApplyTotalModifiersToTarget(); // 모디파이어 다시 합산하고 대상에 적용
			return true;
		}
	}

	return false;
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

void UEffectComponent::ApplyTotalModifiersToTarget()
{
	// 현재 적용된 이팩트의 모디파이어를 합산하고 대상에게 적용
	TotalModifiers.Empty();
	int32 EffectCount = static_cast<int32>(EEffectModifier::COUNT);	// 모든 버프 모디파이어의 개수
	for (auto& Effect : EffectList)
	{
		// EffectList에 있는 모든 이팩트를 순회하면서 모디파이어를 곱연산으로 합산한다.
		for (int32 i = 0; i < EffectCount; i++)
		{
			EEffectModifier ModifierType = static_cast<EEffectModifier>(i);
			float ModifierValue = Effect->GetModifierValue(ModifierType);
			if (ModifierValue != 0.0f)	// 모디파이어 값이 0이 아닐 때만 추가하거나 적용한다.
			{
				if (TotalModifiers.Contains(ModifierType))
				{
					TotalModifiers[ModifierType] *= ModifierValue;	// 서로 다른 이팩트의 모디파이어는 곱연산으로 적용.
				}
				else
				{
					TotalModifiers.Add(ModifierType, ModifierValue);
				}
			}
		}
	}
	if (EffectTarget)
		EffectTarget->ApplyModifiers(&TotalModifiers);	// 대상에게 모디파이어 값을 적용하기
}

void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i < EffectList.Num(); i++)
	{
		EffectList[i]->Tick(DeltaTime);	// 이팩트 틱 진행
	}
}

