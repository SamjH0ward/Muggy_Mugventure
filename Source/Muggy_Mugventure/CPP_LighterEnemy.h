// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_LighterEnemy.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	float FireRate{};

	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	int Damage{};

	bool ShouldAttackPlayer{false};

	bool CanDetectPlayer{false};

	UPROPERTY(EditDefaultsOnly, Category="Shooting", meta = (AllowPrivateAccess = "true"));
	class ACharacter* PlayerPointer;

	UPROPERTY(EditDefaultsOnly, Category="Movement", meta = (AllowPrivateAccess = "true"));
	float RotationSpeed{};



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LighterMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionRadius;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleLeavingDetectionRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Fire(){};
	void RotateToPlayer(FVector lookAtTarget);


};
