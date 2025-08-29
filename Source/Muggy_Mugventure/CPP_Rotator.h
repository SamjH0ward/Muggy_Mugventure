// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Rotator.generated.h"

UENUM(BlueprintType)
enum class ERotatorUseType : uint8
{
	Normal,
	ReturnToMinPoint,
	UseMaxRotationAngleBothWays
};

UCLASS()
class MUGGY_MUGVENTURE_API ACPP_Rotator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Rotator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ToolTip="Normal: Rotate in one direction constantly\nReturnToMinPoint: Rotates to the value set in MaxRotationAngle and then back to the MinRotationAngle\nUseMaxRotationAngleBothWays: Rotates to the MaxRotationAngle and then flips the sign resulting in a pendulum movement"))
	ERotatorUseType RotatorUseMethod;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ClampMin="-179.0",ClampMax="179.0"))
	FRotator RotationVelocity{};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ClampMin="-179.0",ClampMax="179.0", ToolTip="Used for the Return To Min Point option in the Rotator Use Method"))
	FRotator MinRotationAngle{};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true", ClampMin="-179.0",ClampMax="179.0"))
	FRotator MaxRotationAngle{};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	bool bUseOriginMinRotAngle{true};

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	float WaitTime{0.0f};


	

	UPROPERTY()
	FRotator ZeroRot{};

	UFUNCTION()
	void RotateObjectWithBackTrack(float dT);

	UFUNCTION()
	void RotateObjectWithMinRot(float dT);

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

	bool bVelocityIsPitch{false};

	bool ShouldRot{true};

	void ResetShouldRot();

	void CheckWaitCondition();

	FRotator ClampValue(FRotator InRotation, FRotator InClampValue);

	FTimerHandle WaitTimer;



};
