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

protected:
	UFUNCTION(BlueprintCallable)
	void LookMouseLocation(float InDeltaTime);

	UFUNCTION(BlueprintCallable)
	void WarpToOtherSide();

	// 발사체 종류
	enum class EProjectileType : uint8
	{
		Normal,
		Homing,
		Area,
	};

	virtual void OnFireStart(EProjectileType Type);
	virtual void OnFireEnd(EProjectileType Type);

private:
	void GeoInputMove(const FInputActionValue& Value);
	void GeoInputFireNormal_Press(const FInputActionValue& Value);
	void GeoInputFireNormal_Release(const FInputActionValue& Value);
	void GeoInputFireHomming(const FInputActionValue& Value);
	void GeoInputFireArea(const FInputActionValue& Value, bool bPressed);

	// 인라인함수 : 컴파일전에 함수의 바디가 호출부분에 대체된다.
	// __forceinline : 인라인 함수로 처리하도록 설정하는 키워드(반드시는 아님)	
	inline void GetOtherSideLocation(FVector& InOutLocation)
	{
		InOutLocation.X *= -1;
		InOutLocation.Y *= -1;
	}

	void UpdateOrthoSize();

protected:
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

	// 플레이어의 대체적인 크기(보더 위치 결정용)
	const float PlayerMargin = 100.0f;

	// 플레이어가 순간이동하게 되는 경계
	float BorderX = 500.0f + PlayerMargin;
	float BorderY = 888.0f + PlayerMargin;


};
