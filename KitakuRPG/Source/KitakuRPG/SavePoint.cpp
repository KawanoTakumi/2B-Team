// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"
#include "Components/StaticMeshComponent.h"//スタティックメッシュ作成に必要
#include "Components/BoxComponent.h"
#include "MyPlayCharacter.h"

// Sets default values
ASavePoint::ASavePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = BoxMesh;

	hit_collision = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerSphere"));
	hit_collision->SetupAttachment(RootComponent);
	hit_collision->SetCollisionProfileName(TEXT("Trigger"));

	hit_collision->OnComponentBeginOverlap.AddDynamic(this, &ASavePoint::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	point = this->GetActorLocation();
	point.Z += 10.0f;//少し上げる
}

// Called every frame
void ASavePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ASavePoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//タイプに応じて挙動を変更させる
		if (move == MoveType::SAVE)
		{
			AMyPlayCharacter* player = Cast<AMyPlayCharacter>(OtherActor);
			player->startPos = point;
		}
		else if (move == MoveType::BREAK)
		{
			//アクターが設定されていたらそれを破壊する
			if (BreakActor != nullptr)
				BreakActor->Destroy();
		}
	}
}