// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Rotator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACPP_Rotator::ACPP_Rotator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACPP_Rotator::BeginPlay()
{
	Super::BeginPlay();
	if(bUseOriginMinRotAngle) MinRotationAngle = GetActorRotation();
	if(RotationVelocity.Pitch != 0.f) bVelocityIsPitch = true;
	CheckWaitCondition();

}


// Called every frame
void ACPP_Rotator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!ShouldRot)
		return;
	
	switch (RotatorUseMethod)
	{
	case ERotatorUseType::Normal:
		RotateObject(DeltaTime);
		break;

	case  ERotatorUseType::ReturnToMinPoint:
		RotateObjectWithMinRot(DeltaTime);
		break;

	case ERotatorUseType::UseMaxRotationAngleBothWays:
		RotateObjectWithBackTrack(DeltaTime);
		break;

	default:
		RotateObject(DeltaTime);
		break;
	}

	

}

void ACPP_Rotator::RotateObjectWithBackTrack(float dT)
{

	FRotator Test = GetActorRotation() + RotationVelocity*dT;
	Test = ClampValue(Test, MaxRotationAngle);

	//UE_LOG(LogTemp, Display, TEXT("%f, %f, %f"), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);


	SetActorRotation(Test);

	if(IsRotMoreThanZero(RotationVelocity) && IsRotXLessOrEqualToY(MaxRotationAngle,GetActorRotation()))
	{
		FlipTargetLocationAndVelocity();
	}
	else if (IsRotLessThanZero(RotationVelocity) && IsRotXLessOrEqualToY(GetActorRotation(),MaxRotationAngle))
	{
		FlipTargetLocationAndVelocity();
	}//No else as nothing should be done if the velocity is 0 
	
}

void ACPP_Rotator::RotateObjectWithMinRot(float dT)
{
	FRotator Test = GetActorRotation() + RotationVelocity*dT;
	Test = IsRotMoreThanZero(RotationVelocity) ?  ClampValue(Test, MaxRotationAngle) : ClampValue(Test, MinRotationAngle);

	SetActorRotation(Test);

	if(IsRotMoreThanZero(RotationVelocity) && IsRotXLessOrEqualToY(MaxRotationAngle, GetActorRotation()))
	{
		RotationVelocity = ReturnNegativedFRotator(RotationVelocity);
		CheckWaitCondition();
	}
	else if (IsRotLessThanZero(RotationVelocity) && IsRotXLessOrEqualToY(GetActorRotation(), MinRotationAngle))
	{
		RotationVelocity = ReturnNegativedFRotator(RotationVelocity);
		CheckWaitCondition();
	}//No else as nothing should be done if the velocity is 0 

}

void ACPP_Rotator::RotateObject(float dT)
{
	AddActorLocalRotation(RotationVelocity*dT);
}

bool ACPP_Rotator::IsRotMoreThanZero(FRotator InRot)
{
	if(InRot.Yaw > 0) return true;
	if(InRot.Pitch > 0) return true;
	if(InRot.Roll > 0) return true;


	return false;
}

bool ACPP_Rotator::IsRotLessThanZero(FRotator InRot)
{
	if(InRot.Yaw < 0) return true;
	if(InRot.Pitch < 0) return true;
	if(InRot.Roll < 0) return true;

	return false;
}

bool ACPP_Rotator::IsRotXLessOrEqualToY(FRotator InRotX, FRotator InRotY)
{
	//Used to resolve all values being 0
	if(FMath::IsNearlyEqual(InRotX.Yaw,InRotY.Yaw) && FMath::IsNearlyEqual(InRotX.Pitch,InRotY.Pitch) && FMath::IsNearlyEqual(InRotX.Roll,InRotY.Roll)) return true;
	if(bVelocityIsPitch && FMath::IsNearlyEqual(InRotX.Pitch,InRotY.Pitch)) return true; //Fixes an issues where rotating on pitch does wired shit


    return false;
}

FRotator ACPP_Rotator::ReturnNegativedFRotator(FRotator InRot)
{	
	InRot.Yaw = -InRot.Yaw;
	InRot.Pitch = -InRot.Pitch;
	InRot.Roll = -InRot.Roll;
    return InRot;
}


void ACPP_Rotator::FlipTargetLocationAndVelocity()
{
	MaxRotationAngle = ReturnNegativedFRotator(MaxRotationAngle);
	RotationVelocity = ReturnNegativedFRotator(RotationVelocity);
}

void ACPP_Rotator::ResetShouldRot()
{
	ShouldRot = true;
}

void ACPP_Rotator::CheckWaitCondition()
{
	if(WaitTime>0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(WaitTimer, this, &ACPP_Rotator::ResetShouldRot, WaitTime, false);
		ShouldRot = false;
	}
}

FRotator ACPP_Rotator::ClampValue(FRotator InRotation, FRotator InClampValue)
{
	if(IsRotMoreThanZero(RotationVelocity))
	{
		InRotation.Yaw = InRotation.Yaw > InClampValue.Yaw ?  InClampValue.Yaw : InRotation.Yaw;
		InRotation.Pitch = InRotation.Pitch > InClampValue.Pitch ?  InClampValue.Pitch : InRotation.Pitch;
		InRotation.Roll = InRotation.Roll > InClampValue.Roll ?  InClampValue.Roll : InRotation.Roll;

	}
	else
	{
		InRotation.Yaw = InRotation.Yaw < InClampValue.Yaw ?  InClampValue.Yaw : InRotation.Yaw;
		InRotation.Pitch = InRotation.Pitch < InClampValue.Pitch ?  InClampValue.Pitch : InRotation.Pitch;
		InRotation.Roll = InRotation.Roll < InClampValue.Roll ?  InClampValue.Roll : InRotation.Roll;
	}

    return InRotation;
}
