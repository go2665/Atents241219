// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	void GeoInputFireNormal(const FInputActionValue& Value);
	void GeoInputFireHomming(const FInputActionValue& Value);
	void GeoInputFireArea(const FInputActionValue& Value);

protected:
	UFUNCTION(BlueprintCallable)
	void LookMouseLocation(float InDeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* FloatingMovement = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* GeoMoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* GeoFireNormal = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* GeoFireHomming = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* GeoFireArea = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float TurnSpeed = 5.0f;

	


};
