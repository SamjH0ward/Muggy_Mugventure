// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Rotator.h"

// Sets default values
ACPP_Rotator::ACPP_Rotator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called every frame
void ACPP_Rotator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(bShouldRotateBothWays)
	{
		RotateObjectWithBackTrack(DeltaTime);
	}
	else
	{
		RotateObject(DeltaTime);
	}

	

}

void ACPP_Rotator::RotateObjectWithBackTrack(float dT)
{

	FRotator Test = GetActorRotation() + RotationVelocity*dT;
	Test = ClampValue(Test, MaxRotationAngle);


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
	if(InRotX.Yaw != 0 && InRotX.Yaw <= InRotY.Yaw) return true;
	if(InRotX.Pitch != 0 && InRotX.Pitch <= InRotY.Pitch) return true;
	if(InRotX.Roll != 0 && InRotX.Roll <= InRotY.Roll) return true;

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

FRotator ACPP_Rotator::ClampValue(FRotator InRotation, FRotator InClampValue)
{
	if(IsRotMoreThanZero(InClampValue))
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
