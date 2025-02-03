// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	// 액터의 틱을 끌지 말지 결정하는 변수(true면 틱을 사용함, false면 사용하지 않음)

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();	// 부모 클래스의 BeginPlay 함수를 호출함
	
	if (GEngine)
	{
		// 화면에 디버그 메시지를 출력하는 함수. 블루프린트의 Print String과 같은 기능
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Hello World!"));	
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, 
			FString::Printf(TEXT("Hello World! %d %d %d"), MyInt1, MyInt2, MyInt3));	
	}

	UE_LOG(LogTemp, Warning, TEXT("Hello World! 한글"));	// 로그를 출력하는 함수. 에티터의 콘솔에 출력됨

	Test1();
	Test4();
	Test5();
}

void ATestActor::Test1()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hello Test1"));
}

void ATestActor::Test2()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hello Test2"));
}

int32 ATestActor::Test3()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hello Test3"));
	return 1;
}

void ATestActor::Test5_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Hello Test5"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

