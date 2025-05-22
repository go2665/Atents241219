// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"
#include "PoolableActor.h"

void UObjectPoolSubsystem::InitializeObjectPool(const UObjectPoolDataAsset* InData)
{
	for (const FObjectPoolData& Data : InData->ObjectPoolData)
	{
		// ObjectClass에 저장
		ObjectClass.Add(Data.Type, Data.ObjectClass);

		// Pool에 저장
		FActorArray NewActorArray;
		NewActorArray.Actors.Reserve(Data.PoolSize); // 미리 메모리 할당
		//TArray<AActor*> NewPool;
		//NewPool.Reserve(Data.PoolSize); // 미리 메모리 할당
		for (int32 i = 0; i < Data.PoolSize; ++i)
		{
			AActor* NewObject = GetWorld()->SpawnActor<AActor>(Data.ObjectClass, DefaultSpawnLocation, FRotator::ZeroRotator);
			NewObject->SetActorHiddenInGame(true);
			NewObject->SetActorEnableCollision(false);

			if (IPoolableActor* PoolableActor = Cast<IPoolableActor>(NewObject))
			{
				PoolableActor->OnInitialize(); // 인터페이스를 상속 받았다면 오브젝트 초기화 처리 실행
				PoolableActor->OnDeactivate(); // 비활성화 처리
			}
			
			NewActorArray.Actors.Add(NewObject);
						
#if WITH_EDITOR
			FString EnumName = *UEnum::GetValueAsString(Data.Type);
			EnumName.Split(TEXT("::"), nullptr, &EnumName); // EnumName에서 :: 앞부분 제거

            FString Path = FString::Printf(TEXT("ObjectPool/%s"), *EnumName);
			NewObject->SetFolderPath(FName(Path));
#endif
		}
		Pool.Add(Data.Type, NewActorArray);
		ActivatedObjects.Add(Data.Type, FActorArray());
	}
}

AActor* UObjectPoolSubsystem::GetObject(EPooledActorType InType)
{
	AActor* PoolObject = nullptr;
	if (Pool.Contains(InType))
	{		
		TArray<AActor*>& ObjectPool = Pool[InType].Actors;		
		if (ObjectPool.Num() > 0)
		{
			// 풀에서 꺼낼 오브젝트가 있는 경우 마지막 오브젝트를 꺼내서 사용
			PoolObject = ObjectPool.Pop();			
		}
		else
		{
			// 풀에서 꺼낼 오브젝트가 없는 경우 새로 생성해서 사용
			PoolObject = GetWorld()->SpawnActor<AActor>(
				ObjectClass[InType], DefaultSpawnLocation, FRotator::ZeroRotator);
#if WITH_EDITOR
			FString EnumName = *UEnum::GetValueAsString(InType);
			EnumName.Split(TEXT("::"), nullptr, &EnumName); // EnumName에서 :: 앞부분 제거

			FString Path = FString::Printf(TEXT("ObjectPool/%s"), *EnumName);
			PoolObject->SetFolderPath(FName(Path));
#endif
			IPoolableActor* PoolableActor = Cast<IPoolableActor>(PoolObject);
			if (PoolableActor)
			{
				PoolableActor->OnInitialize();
			}
			UE_LOG(LogTemp, Warning, TEXT("ObjectPoolSubsystem: New Object Created!"));
		}		
		PoolObject->SetActorTickEnabled(true);
		PoolObject->SetActorHiddenInGame(false);
		PoolObject->SetActorEnableCollision(true);
		ActivatedObjects[InType].Actors.Add(PoolObject);

		IPoolableActor* PoolableActor = Cast<IPoolableActor>(PoolObject);
		if (PoolableActor)
		{
			PoolableActor->SetPoolType(InType); // 풀 타입 설정
			PoolableActor->OnActivate(); // 인터페이스를 상속 받았다면 오브젝트 활성화 처리 실행
		}			
	}

	return PoolObject;
}

void UObjectPoolSubsystem::ReleaseObject(TScriptInterface<IPoolableActor> InObject)
{
	EPooledActorType Type = InObject->GetPoolType();
	if (ActivatedObjects.Contains(Type))
	{
		AActor* PoolObject = Cast<AActor>(InObject.GetObject());
		if (ActivatedObjects[Type].Actors.Remove(PoolObject) > 0)
		{
			// 풀에서 꺼내진 오브젝트가 맞다.
			Pool[Type].Actors.Add(PoolObject); // 풀에 다시 넣기
			PoolObject->SetActorTickEnabled(false);
			PoolObject->SetActorHiddenInGame(true);
			PoolObject->SetActorEnableCollision(false);
			PoolObject->SetActorLocation(DefaultSpawnLocation); // 기본 위치로 이동

			InObject->OnDeactivate();
		}
	}
}

UObject* UObjectPoolSubsystem::GetDefaultObject(EPooledActorType InType)
{
	if (ObjectClass.Contains(InType))
	{
		return ObjectClass[InType]->GetDefaultObject();
	}
	return nullptr;
}

void UObjectPoolSubsystem::Test()
{
	UE_LOG(LogTemp, Warning, TEXT("ObjectPoolSubsystem Test"));
}
