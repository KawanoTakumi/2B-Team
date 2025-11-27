// Fill out your copyright notice in the Description page of Project Settings.


#include "EBullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyAction.h"
#include "MyPlayCharacter.h"
// Sets default values
AEBullet::AEBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BulletMesh;

	hitCollision = CreateDefaultSubobject<USphereComponent>("HitCollision");
	hitCollision->SetupAttachment(RootComponent);
	hitCollision->SetSphereRadius(30.0f);
	hitCollision->SetCollisionProfileName(TEXT("Trigger"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 400.f;
	ProjectileMovement->MaxSpeed = 800.f;
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
	if (OtherActor->ActorHasTag("Player"))
	{
		if (!OtherActor) return;
		AMyPlayCharacter* TargetPlayer = Cast<AMyPlayCharacter>(OtherActor);
		//プレイヤーが見つかっているかつ、ジャンプできる状態であればダメージを与える
		if (TargetPlayer)
		{
			TargetPlayer->GetDamage(m_damage);
		}
		this->Destroy();
	}
}
void AEBullet::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}