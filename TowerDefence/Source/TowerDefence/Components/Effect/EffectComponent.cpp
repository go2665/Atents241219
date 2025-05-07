// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

bool UEffectComponent::AddEffect(EEffectType InType, int32 InLevel)
{
	// 버프를 생성하고 리스트에 추가한다.
	
	const UEffectDataAsset* EffectData = nullptr;
	if (EffectDataMap.Contains(InType))		// InType이 DataMap에 설정되어 있어야 동작
	{
		EffectData = EffectDataMap[InType];	// 이팩트 데이터 에셋을 찾는다.

		// 최대 스택 회수 구하기
		int32 Level = FMath::Min(InLevel, EffectData->LevelData.Num() - 1); // 안전한 레벨 찾기
		int32 MaxStackCount = EffectData->LevelData[Level].MaxStackCount;	// 최대 스택 회수 가져오기

		// 이팩트가 들어갈 배열 구하기
		if (!EffectTypeMap.Contains(InType))			
		{
			// 이팩트 타입에 해당하는 이팩트 배열 없으면 새로 생성
			FEffectArray NewEffectArray;
			EffectTypeMap.Add(InType, NewEffectArray);
		}
		FEffectArray* EffectArray = EffectTypeMap.Find(InType);	// EffectArray 가져오기
				
		// 최대 스택 회수보다 많으면 가장 먼저 들어와 있던 이팩트를 제거
		if (EffectArray->EffectList.Num() >= MaxStackCount)
		{
			UEffectBase* OldEffect = EffectArray->EffectList[0];	// 가장 먼저 들어온 이팩트 가져오기
			RemoveTargetEffect(OldEffect);							// 이팩트 제거
		}

		// 이팩트 새로 추가
		UEffectBase* NewEffect = CreateEffect(InType);	// 이팩트 생성
		if (NewEffect)
		{
			NewEffect->OnInitialize(InType, EffectData, Level, GetOwner());	// 이팩트 데이터 에셋 설정하면서 초기화
				
			// 필요한 경우 디버그 정보 출력
			if (bShowDebugInfo)
			{
				int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
				float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
				FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
				UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Get New Effect => [%s]"),
					*TimeString,
					*GetOwner()->GetActorNameOrLabel(),
					*NewEffect->GetName());
			}

			// 이팩트 만료 델리게이트 바인딩
			NewEffect->OnEffectExpire.BindLambda([this](UEffectBase* ExpiredEffect)
				{
					RemoveTargetEffect(ExpiredEffect);
				}
			);	

			// 버프 시작
			NewEffect->OnBegin();				

			// 재생중인 특수효과가 없고, 특수효과 데이터가 준비되어 있다면 특수효과 시작
			if (!EffectArray->EffectVFX && EffectData->EffectVFX)
			{	
				EffectArray->EffectVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					EffectData->EffectVFX,
					EffectTarget->GetEffectLocation(),
					FRotator::ZeroRotator,
					FVector(1.0f),
					true,
					true,
					ENCPoolMethod::None,
					true);
			}

			// 이팩트 배열에 추가
			EffectArray->EffectList.Add(NewEffect);			

			// 모디파이어 다시 합산하고 대상에 적용
			ApplyTotalModifiersToTarget();

			return true;
		}
		
	}

	return false;
}

bool UEffectComponent::RemoveEffect(EEffectType InType)
{
	if (FEffectArray* EffectArray = EffectTypeMap.Find(InType))
	{
		for (const auto& Effect : EffectArray->EffectList)
		{
			Effect->OnEnd();	// 이팩트 종료
		}
		EffectArray->EffectList.Empty();	// 이팩트 리스트 비우기
		if (EffectArray->EffectVFX)			// 특수효과가 있으면 특수효과도 지우기
		{
			EffectArray->EffectVFX->Deactivate();		// 이팩트 비활성화
			EffectArray->EffectVFX = nullptr;	// 특수효과 비우기
		}
		ApplyTotalModifiersToTarget();		// 모디파이어 다시 합산하고 대상에 적용

		return true;
	}

	return false;
}

bool UEffectComponent::RemoveTargetEffect(UEffectBase* InRemoveEffect)
{
	if (FEffectArray* EffectArray = EffectTypeMap.Find(InRemoveEffect->GetEffectType()))
	{
		if (EffectArray->EffectList.Contains(InRemoveEffect))
		{
			EffectArray->EffectList.RemoveSingle(InRemoveEffect);	// 이팩트 리스트에서 제거

			// 이팩트가 모두 끝났고 특수효과가 있으면 특수효과도 지우기
			if (EffectArray->EffectList.IsEmpty() && EffectArray->EffectVFX)
			{
				EffectArray->EffectVFX->Deactivate();		// 이팩트 비활성화
				EffectArray->EffectVFX = nullptr;
			}

			// 필요한 경우 디버그 정보 출력
			if (bShowDebugInfo)
			{
				int Minutes = FMath::FloorToInt(GetWorld()->TimeSeconds / 60);
				float Seconds = FMath::Fmod(GetWorld()->TimeSeconds, 60.0f);
				FString TimeString = FString::Printf(TEXT("%02d:%05.2f"), Minutes, Seconds);
				UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Remove Effect => [%s]"),
					*TimeString,
					*GetOwner()->GetActorNameOrLabel(),
					*InRemoveEffect->GetName());
			}

			ApplyTotalModifiersToTarget();		// 모디파이어 다시 합산하고 대상에 적용

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
		// 이팩트 타입을 문자열로 변환하고 시리얼 넘버를 붙여서 이름을 만든다.
		FName EffectName = FName(FString::Printf(TEXT("%s_%d"), *UEnum::GetValueAsString(InType), SerialNumber));
		NewEffect = NewObject<UEffectBase>(this, EffectData->EffectClass, EffectName);
		SerialNumber++;
	}

	return NewEffect;
}

void UEffectComponent::ApplyTotalModifiersToTarget()
{
	// 현재 적용된 모든 이팩트의 모디파이어를 합산하고 대상에게 적용
	TotalModifiers.Empty();
	int32 ModifierCount = static_cast<int32>(EEffectModifier::COUNT);	// 모든 버프 모디파이어의 개수

	for (auto& EffectTypePair : EffectTypeMap)			// 이팩트 타입별로 처리
	{
		// 이팩트 종류별로 모디파이어 합치기(합연산)
		TMap<EEffectModifier, float> EffectModifiers;	// 한 종류의 이팩트의 모디파이어들을 합쳐서 저장할 맵(합연산)
		FEffectArray& EffectArray = EffectTypePair.Value;
		for (auto& Effect : EffectArray.EffectList)		// 이팩트 타입별로 이팩트 리스트를 순회해서
		{
			for (int32 i = 0; i < ModifierCount; i++)	// 모디파이어 종류별로 확인하기 위해 순회
			{
				EEffectModifier ModifierType = static_cast<EEffectModifier>(i);
				float ModifierValue = Effect->GetModifierValue(ModifierType);	// 모디파이어 값 가져오기
				if (ModifierValue != 0.0f)	// 모디파이어 값이 0이 아닐 때만 추가하거나 적용한다.
				{
					if (EffectModifiers.Contains(ModifierType)) 
					{
						EffectModifiers[ModifierType] += ModifierValue; // 이미 있으면 더하기
					}
					else
					{
						EffectModifiers.Add(ModifierType, ModifierValue); // 없으면 추가하기
					}
				}
			}
		}

		// 한 종류의 Effect가 완료되었으면 TotalModifiers에 적용(곱연산)
		for (int32 i = 0; i < ModifierCount; i++)
		{
			EEffectModifier ModifierType = static_cast<EEffectModifier>(i); // 모디파이어 별로 순회
			if (EffectModifiers.Contains(ModifierType)) // 해당 모디파이어가 있으면 TotalModifiers에 적용
			{
				if (TotalModifiers.Contains(ModifierType))
				{
					// TotalModifiers에 다른 이팩트가 추가된 적이 있으면 곱연산으로 추가 적용
					TotalModifiers[ModifierType] += EffectModifiers[ModifierType];
				}
				else
				{
					// TotalModifiers에 다른 이팩트가 추가된 적이 없으면 그냥 추가
					TotalModifiers.Add(ModifierType, EffectModifiers[ModifierType]);
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

	for (auto& EffectTypePair : EffectTypeMap) // 타입별로
	{
		FEffectArray& EffectArray = EffectTypePair.Value;
		for (int32 i = 0; i < EffectArray.EffectList.Num(); i++) // 이팩트 리스트를 순회해서
		{
			EffectArray.EffectList[i]->Tick(DeltaTime);	// 이팩트 틱 진행
		}

		if (EffectArray.EffectVFX) // 특수효과가 재생 중이면
		{
			EffectArray.EffectVFX->SetWorldLocation(EffectTarget->GetEffectLocation());	// 이팩트 위치 업데이트
		}
	}
}

