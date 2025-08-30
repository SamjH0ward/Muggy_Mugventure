// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_LighterEnemy.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "CPP_Fireball.h"


// Sets default values
ACPP_LighterEnemy::ACPP_LighterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LighterRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = LighterRootComponent;


	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComp->SetupAttachment(LighterRootComponent);


	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapsuleComp->SetupAttachment(LighterRootComponent);
	CapsuleComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	

	LighterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lighter"));
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

	ShootingRange = DetectionRadius->GetScaledSphereRadius();

	CurrentWindowToShootAnyWay = WindowToShootAnyWay;

	DetectionRadius->OnComponentBeginOverlap.AddDynamic(this, &ACPP_LighterEnemy::HandleOverlap);

	DetectionRadius->OnComponentEndOverlap.AddDynamic(this,  &ACPP_LighterEnemy::HandleLeavingDetectionRadius);
}

void ACPP_LighterEnemy::HandleOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(OtherActor->ActorHasTag("Player") && CurrentSate==ECurrentBehaviourType::Wander) CurrentSate = ECurrentBehaviourType::SearchForPlayer;
}

void ACPP_LighterEnemy::HandleLeavingDetectionRadius(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("Player") && CurrentSate == ECurrentBehaviourType::AttackingPlayer)
	{
		CurrentSate = ECurrentBehaviourType::ReturningToNormal;
		GetWorld()->GetTimerManager().SetTimer(ReturnToNormalTimer,this,&ACPP_LighterEnemy::ReturnToNormal, ReturningToNormalBehaviourLength,false);
	}
	else if(OtherActor->ActorHasTag("Player"))
	{
		ReturnToNormal();
	}
	
}

void ACPP_LighterEnemy::RotateToPlayer(FVector lookAtTarget)
{
	FVector ToTarget = lookAtTarget - LighterMesh->GetComponentLocation();
	FRotator LookAtRotation =  FRotator(0.0f, ToTarget.Rotation().Yaw-90.f,0.0f);
	

	LighterMesh->SetWorldRotation(FMath::RInterpTo(LighterMesh->GetComponentRotation(), 
	LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), RotationSpeed));
}

void ACPP_LighterEnemy::RestCanAttackAgain()
{
	CurrentSate = ECurrentBehaviourType::Wander;
}

// Called every frame
void ACPP_LighterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!PlayerPointer) return;

	switch(CurrentSate)
	{
		case ECurrentBehaviourType::Wander:
			return;
			break;
		case ECurrentBehaviourType::ChaseCoolDown:
			return;
			break;
		case ECurrentBehaviourType::SearchForPlayer:
			SearchForPlayer();
		break;
		case ECurrentBehaviourType::AttackingPlayer:
			RotateToPlayer(PlayerPointer->GetActorLocation());
			CheckIfShouldShoot(DeltaTime);

		break;
		case ECurrentBehaviourType::ReturningToNormal:
			RotateToPlayer(PlayerPointer->GetActorLocation());
		break;
		default:
		break;
	}
	

	
}

void ACPP_LighterEnemy::CheckIfShouldShoot(float DT)
{	
	if(HasShot) return;

	FHitResult HitResult;
	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * ShootingRange;

	bool HasHit = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_GameTraceChannel2);
	if(HasHit && HitResult.GetActor()->ActorHasTag("Player"))
	{	
		
		Fire();
	} 
	else CurrentWindowToShootAnyWay -= 1.0f * DT;

	if(CurrentWindowToShootAnyWay <= 0.0f)
	{
		Fire();
	}
	
}

void ACPP_LighterEnemy::Fire()
{
		HasShot = true;
		ACPP_Fireball* Fireball = GetWorld()->SpawnActor<ACPP_Fireball>(FireballClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		Fireball->SetOwner(this);

		CurrentWindowToShootAnyWay = WindowToShootAnyWay;
		CurrentSate = ECurrentBehaviourType::ReturningToNormal;
		GetWorld()->GetTimerManager().SetTimer(ReturnToNormalTimer,this,&ACPP_LighterEnemy::ReturnToNormal, ReturningToNormalBehaviourLength,false);
}

void ACPP_LighterEnemy::ReturnToNormal()
{
	ShouldFollowSpline = true;
	CurrentWindowToShootAnyWay = WindowToShootAnyWay;
	if(CurrentSate != ECurrentBehaviourType::ChaseCoolDown) 
		GetWorld()->GetTimerManager().SetTimer(ReturnToWanderTimer, this, &ACPP_LighterEnemy::RestCanAttackAgain, ChaseCooldown, false);
	CurrentSate = ECurrentBehaviourType::ChaseCoolDown;

	HasShot = false;

	
}

void ACPP_LighterEnemy::SearchForPlayer()
{
	
	FHitResult HitResult;
	bool HasHit = GetWorld()->LineTraceSingleByChannel(HitResult,GetActorLocation(),PlayerPointer->GetActorLocation(),ECC_GameTraceChannel2);
	
	if(HasHit) 
	{
		CurrentSate = ECurrentBehaviourType::AttackingPlayer;
		ShouldFollowSpline =  false;
	} 
	
}
