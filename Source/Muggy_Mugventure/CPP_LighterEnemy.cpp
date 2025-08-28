// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LighterEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"


// Sets default values
ACPP_LighterEnemy::ACPP_LighterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;
	CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	

	LighterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lighter"));
	LighterMesh->SetupAttachment(CapsuleComp);
	LighterMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(LighterMesh);
	

	DetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Radius"));
	DetectionRadius->SetupAttachment(LighterMesh);
	DetectionRadius->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);

	
}

// Called when the game starts or when spawned
void ACPP_LighterEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerPointer = Cast<ACharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	


	DetectionRadius->OnComponentBeginOverlap.AddDynamic(this, &ACPP_LighterEnemy::HandleOverlap);

	DetectionRadius->OnComponentEndOverlap.AddDynamic(this,  &ACPP_LighterEnemy::HandleLeavingDetectionRadius);
}

void ACPP_LighterEnemy::HandleOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag("Player")) CanDetectPlayer = true;
}

void ACPP_LighterEnemy::HandleLeavingDetectionRadius(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player")) ShouldAttackPlayer = CanDetectPlayer = false;
}

void ACPP_LighterEnemy::RotateToPlayer(FVector lookAtTarget)
{
	FVector ToTarget = lookAtTarget - GetActorLocation();
	FRotator LookAtRotation =  FRotator(0.0f, ToTarget.Rotation().Yaw,0.0f);

	LighterMesh->SetWorldRotation(FMath::RInterpTo(LighterMesh->GetComponentRotation(), 
	LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotationSpeed));

}

// Called every frame
void ACPP_LighterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CanDetectPlayer && !ShouldAttackPlayer)
	{
		FHitResult HitResult;
		bool HasHit = GetWorld()->LineTraceSingleByChannel(HitResult,GetActorLocation(),PlayerPointer->GetActorLocation(),ECC_GameTraceChannel2);
		
		if(HasHit) 
		{
			ShouldAttackPlayer = HitResult.GetActor()->ActorHasTag("Player");
			if(GEngine)
     GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %s"), *HitResult.GetActor()->GetActorNameOrLabel()));
			CanDetectPlayer = !ShouldAttackPlayer;
		} 
	}

	if(PlayerPointer && ShouldAttackPlayer) RotateToPlayer(PlayerPointer->GetActorLocation());



}



