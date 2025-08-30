// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Fireball.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"

// Sets default values
ACPP_Fireball::ACPP_Fireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

}

// Called when the game starts or when spawned
void ACPP_Fireball::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ACPP_Fireball::OnHit);

	GetWorld()->GetTimerManager().SetTimer(LifeTimeTimer,this,&ACPP_Fireball::DestroySelf, LifeTime,false);
}

void  ACPP_Fireball::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	AActor* myOwner = GetOwner();
	if(!myOwner) return;

	if(otherActor && otherActor != this && otherActor != myOwner)
	{
		if(otherActor->ActorHasTag("Player")) DoDamage(otherActor);
		Destroy();
	}


}

void ACPP_Fireball::DestroySelf()
{
	Destroy();
}

// Called every frame
void ACPP_Fireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

