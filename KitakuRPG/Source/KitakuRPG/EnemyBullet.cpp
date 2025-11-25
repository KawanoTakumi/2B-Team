// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "Components/StaticMeshComponent.h"//スタティックメッシュ作成に必要
#include "Components/SphereComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//弾丸のメッシュを設定
	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = bulletMesh;

	//あたり判定、当たった時の挙動を作成
	hitSphere->CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	hitSphere->SetupAttachment(RootComponent);
	hitSphere->InitSphereRadius(200.f); // 範囲を調整
	hitSphere->SetCollisionProfileName(TEXT("Trigger"));
	hitSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::Attacked);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 重力なしなら0

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = FireDirection * ProjectileMovement->InitialSpeed;
	}

}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*攻撃判定用関数*/
void AEnemyBullet::Attacked(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("PLAYER"))
	{

	}
}