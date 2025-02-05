// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

#include "PlayerWing.generated.h"

UCLASS()
class SHOOTINGCPP_API APlayerWing : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerWing();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void GeoInputMove(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* GeoMoveAction = nullptr;


};
