// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantWall.h"
#include "Components/StaticMeshComponent.h"//スタティックメッシュ作成に必要
#include "Components/SphereComponent.h"//球体作成に必要
#include "GameFramework/Character.h"
#include "MyPlayCharacter.h"

// Sets default values
APlantWall::APlantWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetupAttachment(RootComponent);
	TriggerSphere->InitSphereRadius(200.f); // 範囲を調整
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APlantWall::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APlantWall::BeginPlay()
{
	Super::BeginPlay();
	
}

//プレイヤー索敵関数、破壊関数
void APlantWall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<ACharacter>(OtherActor))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			AMyPlayCharacter* TargetPlayer = Cast<AMyPlayCharacter>(OtherActor);
			if (TargetPlayer && TargetPlayer->torchCount > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("player hit"));
				TargetPlayer->torchCount--;//アイテムの所持数を０にする
				this->Destroy();
			}
		}
	}
}