// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionPlayerCharacter.generated.h"

UCLASS()
class ACTION_API AActionPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionPlayerCharacter();

	// 컨트롤러에서 입력 방향을 받아서 처리하는 함수
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	void Movement(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	void DoRoll();

	// 달리기 모드로 설정
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	inline void SetSprintMode() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; };

	// 걷기 모드로 설정
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	inline void SetWalkMode() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Camera")
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Camera")
	class UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float SprintSpeed = 1250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	UAnimMontage* RollMontage = nullptr;

};
