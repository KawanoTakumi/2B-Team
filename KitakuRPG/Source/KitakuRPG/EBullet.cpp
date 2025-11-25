// Fill out your copyright notice in the Description page of Project Settings.


#include "EBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
AEBullet::AEBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;

	hitCollision = CreateDefaultSubobject<USphereComponent>("HitCollision");
	hitCollision->SetupAttachment(RootComponent);
	hitCollision->SetSphereRadius(30.0f);
	hitCollision->SetCollisionProfileName(TEXT("Trigger"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 重力なし


}

// Called when the game starts or when spawned
void AEBullet::BeginPlay()
{
	Super::BeginPlay();

	hitCollision->OnComponentBeginOverlap.AddDynamic(this, &AEBullet::HitBullet);
}

// Called every frame
void AEBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEBullet::HitBullet(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	//このオブジェクトを消す
	this->Destroy();
	if (OtherActor->ActorHasTag("Player"))
	{

		
	}
}
void AEBullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}