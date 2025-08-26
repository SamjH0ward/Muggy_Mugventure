// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Rotator.generated.h"

UCLASS()
class MUGGY_MUGVENTURE_API ACPP_Rotator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Rotator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ClampMin="-179.0",ClampMax="179.0"))
	FRotator RotationVelocity{};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ClampMin="-179.0",ClampMax="179.0"))
	FRotator MaxRotationAngle{};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	bool bShouldRotateBothWays;

	UPROPERTY()
	FRotator ZeroRot{};

	UFUNCTION()
	void RotateObjectWithBackTrack(float dT);

	UFUNCTION()
	void RotateObject(float dT);

	UFUNCTION()
	bool IsRotMoreThanZero(FRotator InRot);

	UFUNCTION()
	bool IsRotLessThanZero(FRotator InRot);

	UFUNCTION()
	bool IsRotXLessOrEqualToY(FRotator InRotX, FRotator InRotY);

	FRotator ReturnNegativedFRotator(FRotator InRot);

	void FlipTargetLocationAndVelocity();

	FRotator ClampValue(FRotator InRotation, FRotator InClampValue);

};
