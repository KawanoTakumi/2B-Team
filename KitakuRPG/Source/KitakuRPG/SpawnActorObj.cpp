// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorObj.h"
#include "Components/SphereComponent.h"//球体コンポーネント
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnActorObj::ASpawnActorObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//索敵用の球体を作成
	hit_collision = CreateDefaultSubobject<USphereComponent>("HitSphere");
	hit_collision->SetupAttachment(RootComponent);
	hit_collision->SetSphereRadius(700.0f);
	hit_collision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ASpawnActorObj::BeginPlay()
{
	Super::BeginPlay();
	hit_collision->OnComponentBeginOverlap.AddDynamic(this, &ASpawnActorObj::ASpawn);
}

// Called every frame
void ASpawnActorObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_spawn_timer > 0)
		m_spawn_timer -= DeltaTime;
	else
	{
		Spawn_Ac();
	}
}

//プレイヤーが当たったらフラグをtrueにする
void ASpawnActorObj::ASpawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	m_can_spawn_actor = true;
}

void ASpawnActorObj::Spawn_Ac()
{
	//スポーン可能になるまでの時間を計測
	if (m_can_spawn_actor && spawn_object)
	{
		if (particle)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), particle, GetActorLocation(), GetActorRotation());
		}
		FRotator SpawnRotation = GetActorRotation();
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation += GetActorUpVector() * 100.0f;
		FActorSpawnParameters param;
		AEnemyAction* Enemy = GetWorld()->SpawnActor<AEnemyAction>(spawn_object, SpawnLocation, SpawnRotation, param);
		if (Enemy)
		{
			Enemy->SpawnDefaultController();
		}
		m_spawn_timer = spawn_interval;
		m_can_spawn_actor = false;
	}
}
