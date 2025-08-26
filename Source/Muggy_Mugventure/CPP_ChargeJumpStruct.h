// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_ChargeJumpStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FChargeJumpValues
{
	 GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCharge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
 	int HealthCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0",ClampMax="90.0"))
	float LaunchAngle;
 	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0",ClampMax="1.0"))
	float SlowDownPercentage;
};
	