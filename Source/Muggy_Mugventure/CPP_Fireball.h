// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_Fireball.generated.h"

UCLASS()
class MUGGY_MUGVENTURE_API ACPP_Fireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Fireball();

	UFUNCTION(BlueprintImplementableEvent)
	void DoDamage(AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	int Damage{20};



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

UPROPERTY(VisibleDefaultsOnly, Category="Components", meta=( AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh; 

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float LifeTime;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	FTimerHandle LifeTimeTimer;

	void DestroySelf();


	
};
