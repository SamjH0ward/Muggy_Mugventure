// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_LighterEnemy.generated.h"

UENUM(BlueprintType)
enum class ECurrentBehaviourType : uint8
{
	Wander,
	SearchForPlayer,
	AttackingPlayer,
	ReturningToNormal
};

class USceneComponent;

UCLASS()
class MUGGY_MUGVENTURE_API ACPP_LighterEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_LighterEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/*----------------------
			Methods
	-------------------------*/
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleLeavingDetectionRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckIfShouldShoot(float DT);
	void Fire();
	void ReturnToNormal();
	void SearchForPlayer();
	void RotateToPlayer(FVector lookAtTarget);

	/*----------------------
		  Components
	-------------------------*/


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LighterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* LighterRootComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionRadius;


	
	/*----------------------
		  Exposed Vars
	-------------------------*/


	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	float FireRate{};

	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	int Damage{};

	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	float WindowToShootAnyWay{3.0f};
	
	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	class ACharacter* PlayerPointer;

	UPROPERTY(EditDefaultsOnly, Category="Behaviour", meta = (AllowPrivateAccess = "true"));
	float ReturningToNormalBehaviourLength{5.0f};

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Behaviour", meta = (AllowPrivateAccess = "true"));
	bool ShouldFollowSpline{true};

	UPROPERTY(EditDefaultsOnly, Category="Movement", meta = (AllowPrivateAccess = "true"));
	float RotationSpeed{};

	/*----------------------
		  Internal Vars
	-------------------------*/

	ECurrentBehaviourType CurrentSate{ECurrentBehaviourType::Wander};
	
	bool ShootAnyWay{false};

	float CurrentWindowToShootAnyWay{3.0f};

	float ShootingRange{500.f};

	UPROPERTY()
	FTimerHandle ReturnToNormalTimer; 


};
