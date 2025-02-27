// Fill out your copyright notice in the Description page of Project Settings.


#include "Exam03_Go.h"


void AExam03_Go::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!DynamicMaterial && StaticMesh)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Exam03Material, this);
		StaticMesh->SetMaterial(0, DynamicMaterial);
	}
}

void AExam03_Go::BeginPlay()
{
	Super::BeginPlay();

	InitEnhancedInput();

	OnTest1Delegate.AddDynamic(this, &AExam03_Go::OnTest1Handler);
	OnTest2Delegate.AddDynamic(this, &AExam03_Go::OnTest2Handler);
}

void AExam03_Go::InitEnhancedInput()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = 
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSubsystem && InputMappingContext)
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
			
			// EnableInput
			UEnhancedInputComponent* EnhancedInputComponent =
				Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(
					InputAction_Test1, ETriggerEvent::Started, this, &AExam03_Go::OnTest1);
				EnhancedInputComponent->BindAction(
					InputAction_Test2, ETriggerEvent::Started, this, &AExam03_Go::OnTest2_Press);
				EnhancedInputComponent->BindAction(
					InputAction_Test2, ETriggerEvent::Completed, this, &AExam03_Go::OnTest2_Release);
			}
		}
	}
}

void AExam03_Go::OnTest1()
{	
	OnTest1Delegate.Broadcast();
}

void AExam03_Go::OnTest2_Press()
{
	PressStartTime = GetWorld()->GetTimeSeconds();
}

void AExam03_Go::OnTest2_Release()
{
	float Duration = GetWorld()->GetTimeSeconds() - PressStartTime;
	OnTest2Delegate.Broadcast(Duration);
}

void AExam03_Go::OnTest1Handler()
{
	DynamicMaterial->SetVectorParameterValue("BaseColor", TestColor);
}

void AExam03_Go::OnTest2Handler(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Duration: %f"), Value));
}
